#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QTextCursor>
#include <QTextTableFormat>
#include <QDebug>
#include <QByteArray>
#include <QMessageBox>
#include <QKeySequence>
#include <QScrollBar>

#include "MainWindow.hpp"
#include "ConfigureDialog.hpp"
#include "Settings.hpp"
#include "Proxy.hpp"

#include <medusa/binary_stream.hpp>
#include <medusa/module.hpp>
#include <medusa/log.hpp>

MainWindow::MainWindow()
  : QMainWindow(), Ui::MainWindow()
  , _about(this)
  , _openConfirmation(this)
  , _goto(this)
  , _settingsDialog(this)
  , _undoJumpView()
  , _fileName("")
  , _documentOpened(false)
  , _closeWindow(false)
  , _openDocument(false)
  , _medusa()
{
  this->setupUi(this);

  medusa::Log::SetLog(boost::bind(&MainWindow::appendLog, this, _1));

  this->tabWidget->setTabsClosable(true);

  this->restoreGeometry(Settings::instance().value(WINDOW_GEOMETRY, WINDOW_GEOMETRY_DEFAULT).toByteArray());
  this->restoreState(Settings::instance().value(WINDOW_LAYOUT, WINDOW_LAYOUT_DEFAULT).toByteArray());

  connect(this->tabWidget, SIGNAL(tabCloseRequested(int)),       this, SLOT(on_tabWidget_tabCloseRequested(int)));
  connect(this,            SIGNAL(logAppended(QString const &)), this, SLOT(onLogMessageAppended(QString const &)));
}

MainWindow::~MainWindow()
{
}

bool MainWindow::openDocument()
{
  _fileName = QFileDialog::getOpenFileName(this, tr("Select a file"));
  if (_fileName.isNull())
    return false;

  return _medusa.NewDocument(_fileName.toStdWString(),
    [&](boost::filesystem::path& dbPath, std::list<medusa::Medusa::Filter> const& filters)
  {
    dbPath = QFileDialog::getSaveFileName(this,
      "Select a database path",
      QString::fromStdWString(dbPath.wstring())
      ).toStdWString();
    return true;
  },
    [&](medusa::BinaryStream::SharedPtr bs, medusa::Database::SharedPtr& db, medusa::Loader::SharedPtr& ldr, medusa::Architecture::VectorSharedPtr& archs, medusa::OperatingSystem::SharedPtr& os)
  {
    ConfigureDialog CfgDlg(this, bs);
    CfgDlg.exec();

    medusa::Architecture::SharedPtr arch;
    LoaderChooser lc(this, _medusa);
    if (!lc.getSelection(ldr, arch, os, db))
    {
      this->closeDocument();
      return false;
    }
    archs.push_back(arch);
    return true;
  },
    [&](void)
  {
    // Widgets initialisation must be called before file mapping... Except scrollbar address
    auto memAreaView = new MemoryAreaView(this, _medusa);
    this->memAreaDock->setWidget(memAreaView);
    connect(memAreaView, SIGNAL(goTo(medusa::Address const&)), this, SLOT(goTo(medusa::Address const&)));

    auto labelView = new LabelView(this, _medusa);
    this->labelDock->setWidget(labelView);
    connect(labelView,   SIGNAL(goTo(medusa::Address const&)), this, SLOT(goTo(medusa::Address const&)));

    return true;
  },
    [&]()
  {
    // FIXME If this is placed before mapping, it leads to a div to 0
    auto sbAddr = new ScrollbarAddress(this, _medusa);
    this->addressDock->setWidget(sbAddr);
    connect(sbAddr, SIGNAL(goTo(medusa::Address const&)), this, SLOT(goTo(medusa::Address const&)));
    connect(this, SIGNAL(lastAddressUpdated(medusa::Address const&)), sbAddr, SLOT(setCurrentAddress(medusa::Address const&)));

    this->actionGoto->setEnabled(true);
    this->_documentOpened = true;
    this->setWindowTitle("Medusa - " + this->_fileName);

    addDisassemblyView(_medusa.GetDocument().GetStartAddress());

    return true;
  });
}

bool MainWindow::loadDocument()
{
  auto& modMgr = medusa::ModuleManager::Instance();
  try
  {
    modMgr.UnloadModules();
    modMgr.LoadModules(L"."); // TODO: Let the user select the folder which contains modules

    auto const& allDbs = modMgr.GetDatabases();
    auto endDb = std::end(allDbs);
    QString filters;
    for (auto curDb = std::begin(allDbs); curDb != endDb; ++curDb)
    {
      filters += QString("%1 (*%2) ;;").arg((*curDb)->GetName().c_str(), (*curDb)->GetExtension().c_str());
    }

    _fileName = QFileDialog::getOpenFileName(this, tr("Select a database"), QString(), filters);

    if (_fileName.isNull())
      return false;

    medusa::Database::SharedPtr db = nullptr;
    for (auto curDb = std::begin(allDbs); curDb != endDb; ++curDb)
    {
      if ((*curDb)->IsCompatible(_fileName.toStdWString()))
      {
        db = *curDb;
        break;
      }
    }

    if (db == nullptr)
      throw medusa::Exception(L"unable to find correct module to handle database");

    if (!db->Open(_fileName.toStdWString()))
      throw medusa::Exception(L"unable to open database");

    // Opening file and loading module
    modMgr.UnloadModules();
    modMgr.LoadModules(L".", *db->GetBinaryStream()); // TODO: Let the user select the folder which contains modules

    emit logAppended(QString("Opening %1\n").arg(this->_fileName));

    medusa::Loader::VectorSharedPtr const & loaders = modMgr.GetLoaders();

    // If no compatible loader was found
    if (loaders.empty())
    {
      QMessageBox::critical(this, tr("Loader error"), tr("There is no supported loader for this file"));
      this->closeDocument();
      return false;
    }

    // Select arch
    medusa::Architecture::VectorSharedPtr const & archis = modMgr.GetArchitectures();

    // If no compatible arch was found
    if (archis.empty())
    {
      QMessageBox::critical(this, tr("Architecture error"), tr("There is no supported architecture for this file"));
      this->closeDocument();
      return false;
    }

    medusa::Loader::SharedPtr loader;
    medusa::Architecture::SharedPtr architecture;
    medusa::OperatingSystem::SharedPtr os;
    medusa::Database::SharedPtr newDb;

    LoaderChooser lc(this, _medusa);
    if (!lc.getSelection(loader, architecture, os, newDb))
    {
      this->closeDocument();
      return false;
    }

    // Widgets initialisation must be called before file mapping... Except scrollbar address
    auto memAreaView = new MemoryAreaView(this, _medusa);
    this->memAreaDock->setWidget(memAreaView);

    auto labelView = new LabelView(this, _medusa);
    this->labelDock->setWidget(labelView);

    medusa::Architecture::VectorSharedPtr archs;
    archs.push_back(architecture);
    this->_medusa.Start(db->GetBinaryStream(), db, loader, archs, os);

    // FIXME If this is placed before mapping, it leads to a div to 0
    auto sbAddr = new ScrollbarAddress(this, _medusa);
    this->addressDock->setWidget(sbAddr);

    this->actionGoto->setEnabled(true);
    this->_documentOpened = true;
    this->setWindowTitle("Medusa - " + this->_fileName);

    addDisassemblyView(_medusa.GetDocument().GetStartAddress());

    connect(labelView,   SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
    connect(sbAddr,      SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
    connect(memAreaView, SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
    connect(this,        SIGNAL(lastAddressUpdated(medusa::Address const&)), sbAddr, SLOT(setCurrentAddress(medusa::Address const&)));
  }
  catch (medusa::Exception const& e)
  {
    QMessageBox::critical(this, "Error", QString::fromStdWString(e.What()));
    this->closeDocument();
    return false;
  }

  return true;
}

bool MainWindow::saveDocument()
{
  QMessageBox::critical(this, "Error", "Not implemented");
  return false;
}

bool MainWindow::closeDocument()
{
  this->_documentOpened = false;
  this->actionClose->setEnabled(false);
  this->actionGoto->setEnabled(false);

  this->logEdit->clear();
  auto labelView = this->labelDock->widget();
  disconnect(labelView, SIGNAL(goTo(medusa::Address const&)), this, SLOT(goTo(medusa::Address const&)));
  this->labelDock->setWidget(nullptr);
  delete labelView;

  int tabWidgetCount = this->tabWidget->count();
  for (int i = 0; i < tabWidgetCount; ++i)
  {
    auto curWidget = this->tabWidget->widget(i);
    this->tabWidget->removeTab(i);
    delete curWidget;
  }

  return true;
}

void MainWindow::appendLog(std::wstring const& msg)
{
  emit logAppended(QString::fromStdWString(msg));
}

void MainWindow::addDisassemblyView(medusa::Address const& startAddr)
{
  auto disasmView = new DisassemblyView(this, &_medusa);
  connect(disasmView, SIGNAL(cursorAddressUpdated(medusa::Address const&)), this->addressDock->widget(), SLOT(setCurrentAddress(medusa::Address const&)));
  this->tabWidget->addTab(disasmView, "Disassembly (text)");
  disasmView->goTo(startAddr);
}

void MainWindow::addSemanticView(medusa::Address const& funcAddr)
{
  auto func = _medusa.GetMultiCell(funcAddr);
  if (func == nullptr || func->GetType() != medusa::MultiCell::FunctionType)
    return;

  auto lbl = _medusa.GetDocument().GetLabelFromAddress(funcAddr);
  QString funcLbl = QString::fromStdString(funcAddr.ToString());
  if (lbl.GetType() != medusa::Label::Unknown)
    funcLbl = QString::fromStdString(lbl.GetLabel());

  auto semView = new SemanticView(this, _medusa, funcAddr);
  this->tabWidget->addTab(semView, QString("Semantic of function %1").arg(funcLbl));
}

void MainWindow::addControlFlowGraphView(medusa::Address const& funcAddr)
{
  auto lbl = _medusa.GetDocument().GetLabelFromAddress(funcAddr);
  QString funcLbl = QString::fromStdString(funcAddr.ToString());
  if (lbl.GetType() != medusa::Label::Unknown)
    funcLbl = QString::fromStdString(lbl.GetLabel());

  auto cfgView = new ControlFlowGraphView(this);
  auto cfgScene = new ControlFlowGraphScene(this->tabWidget, _medusa, funcAddr);
  cfgView->setScene(cfgScene);
  this->tabWidget->addTab(cfgView, QString("Graph of function %1").arg(funcLbl));
}

void MainWindow::on_actionAbout_triggered()
{
  this->_about.exec();
}

void MainWindow::on_actionOpen_triggered()
{
  // If a document is already opened,
  // Confirmation dialog
  if (this->_documentOpened)
  {
    if (this->_openConfirmation.exec() == QDialog::Rejected)
      return;

    if (!this->closeDocument())
      return;

    this->_openDocument = true;
  }
  this->openDocument();
}

void MainWindow::on_actionLoad_triggered()
{
  this->loadDocument();
}

void MainWindow::on_actionSave_triggered()
{
  this->saveDocument();
}

void MainWindow::on_actionClose_triggered()
{
  this->closeDocument();
}

void MainWindow::on_actionGoto_triggered()
{
  if (this->_goto.exec() == QDialog::Rejected)
    return;

  auto addr = _goto.address();
  emit goTo(addr);
}

void MainWindow::on_actionDisassembly_triggered()
{
  addDisassemblyView(_medusa.GetDocument().GetAddressFromLabelName("start"));
}

void MainWindow::on_actionSettings_triggered()
{
  this->_settingsDialog.exec();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
  auto curWidget = this->tabWidget->widget(index);
  this->tabWidget->removeTab(index);
  delete curWidget;
}

void MainWindow::onLogMessageAppended(QString const & msg)
{
  logEdit->insertPlainText(msg);
}

void MainWindow::goTo(medusa::Address const& addr)
{
  // LATER: implement goTo method for all views
  auto disasmView = dynamic_cast<DisassemblyView*>(this->tabWidget->currentWidget());
  if (disasmView == nullptr)
    return;
  if (!disasmView->goTo(addr))
  {
    QMessageBox::warning(this, "Invalid address", "This address looks invalid");
  }
  emit lastAddressUpdated(addr);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
  Settings::instance().setValue(WINDOW_LAYOUT, this->saveState());
  Settings::instance().setValue(WINDOW_GEOMETRY, this->saveGeometry());
  event->accept();
}
