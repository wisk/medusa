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
#include <QMessageBox>

#include "MainWindow.hpp"
#include "ConfigureDialog.hpp"
#include "Proxy.hpp"

#include <medusa/binary_stream.hpp>
#include <medusa/module.hpp>
#include <medusa/log.hpp>
#include <medusa/user_configuration.hpp>

MainWindow::MainWindow(QString const& rFilePath, QString const& rDbPath)
  : QMainWindow(), Ui::MainWindow()
  , _about(this)
  , _goto(this)
  , _settingsDialog(this, _medusa)
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

  medusa::UserConfiguration UserCfg;
  std::string Base64Geometry = UserCfg.GetOption("ui_qt.geometry");
  std::string Geometry = medusa::Base64Decode(Base64Geometry);
  restoreGeometry(QByteArray(Geometry.data(), Geometry.size()));
  std::string Base64State = UserCfg.GetOption("ui_qt.state");
  std::string State = medusa::Base64Decode(Base64State);
  restoreState(QByteArray(State.data(), State.size()));

  connect(this->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tabWidget_tabCloseRequested(int)));
  connect(this, SIGNAL(logAppended(QString const &)), this, SLOT(onLogMessageAppended(QString const &)));
  connect(this, SIGNAL(lastAddressUpdated(medusa::Address const&)), this, SLOT(setCurrentAddress(medusa::Address const&)));

  if (rFilePath.isEmpty())
    return;

  // new document
  if (rDbPath.isEmpty())
  {
    _fileName = rFilePath;

    _medusa.NewDocument(std::make_shared<medusa::FileBinaryStream>(_fileName.toStdWString()),
        [&](medusa::Path& rDbPath, std::list<medusa::Medusa::Filter> const& filters)
        {
        auto DbPrDbPathath = QFileDialog::getSaveFileName(this,
            "Select a database path",
            QString::fromStdString(rDbPath.string())
            ).toStdString();
        return true;
        },
        [&](medusa::BinaryStream::SPType bs, medusa::Database::SPType& db, medusa::Loader::SPType& ldr, medusa::Architecture::VSPType& archs, medusa::OperatingSystem::SPType& os)
        {
        ConfigureDialog CfgDlg(this, bs);
        if (CfgDlg.exec() == QDialog::Rejected)
        return false;

        db    = CfgDlg.GetSelectedDatabase();
        ldr   = CfgDlg.GetSelectedLoader();
        archs = CfgDlg.GetSelectedArchitectures();
        os    = CfgDlg.GetSelectedOperatingSystem();

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

          this->actionOpen->setEnabled(false);
          this->actionLoad->setEnabled(false);
          this->actionGoto->setEnabled(true);
          this->actionClose->setEnabled(true);
          this->_documentOpened = true;
          this->setWindowTitle("Medusa - " + this->_fileName);

          addDisassemblyView(_medusa.GetDocument().GetStartAddress());

          return true;
        });

    return;
  }

  // open document
  if (!_medusa.OpenDocument([&](
          medusa::Path& rDatabasePath,
          std::list<medusa::Medusa::Filter> const& rExtensionFilter
          )
        {
          rDatabasePath = rDbPath.toStdWString();
          return true;
        }))
  return;

  // Widgets initialisation must be called before file mapping... Except scrollbar address
  auto memAreaView = new MemoryAreaView(this, _medusa);
  memAreaView->Refresh();
  this->memAreaDock->setWidget(memAreaView);

  auto labelView = new LabelView(this, _medusa);
  labelView->Refresh();
  this->labelDock->setWidget(labelView);

  auto pBindingView = new BindingView(_medusa);
  this->BindingDock->setWidget(pBindingView);

  //medusa::Architecture::VSPType archs;
  //archs.push_back(architecture);
  //this->_medusa.Start(db->GetBinaryStream(), db, loader, archs, os);

  // FIXME If this is placed before mapping, it leads to a div to 0
  auto sbAddr = new ScrollbarAddress(this, _medusa);
  sbAddr->Refresh();
  this->addressDock->setWidget(sbAddr);

  this->actionOpen->setEnabled(false);
  this->actionLoad->setEnabled(false);
  this->actionGoto->setEnabled(true);
  this->actionClose->setEnabled(true);
  this->_documentOpened = true;
  this->setWindowTitle("Medusa - " + this->_fileName);

  addDisassemblyView(_medusa.GetDocument().GetStartAddress());

  connect(labelView,   SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
  connect(sbAddr,      SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
  connect(memAreaView, SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
  connect(this,        SIGNAL(lastAddressUpdated(medusa::Address const&)), sbAddr, SLOT(setCurrentAddress(medusa::Address const&)));
}

MainWindow::~MainWindow()
{
}

bool MainWindow::openDocument()
{
  if (_documentOpened)
    return false;

  _fileName = QFileDialog::getOpenFileName(this, tr("Select a file"));
  if (_fileName.isNull())
    return false;

  return _medusa.NewDocument(std::make_shared<medusa::FileBinaryStream>(_fileName.toStdWString()),
    [&](medusa::Path& rDbPath, std::list<medusa::Medusa::Filter> const& filters)
  {
    auto DbPrDbPathath = QFileDialog::getSaveFileName(this,
      "Select a database path",
      QString::fromStdString(rDbPath.string())
      ).toStdString();
    return true;
  },
    [&](medusa::BinaryStream::SPType bs, medusa::Database::SPType& db, medusa::Loader::SPType& ldr, medusa::Architecture::VSPType& archs, medusa::OperatingSystem::SPType& os)
  {
    ConfigureDialog CfgDlg(this, bs);
    if (CfgDlg.exec() == QDialog::Rejected)
      return false;

    db    = CfgDlg.GetSelectedDatabase();
    ldr   = CfgDlg.GetSelectedLoader();
    archs = CfgDlg.GetSelectedArchitectures();
    os    = CfgDlg.GetSelectedOperatingSystem();

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

    auto pBindingView = new BindingView(_medusa);
    this->BindingDock->setWidget(pBindingView);

    return true;
  },
    [&]()
  {
    // FIXME If this is placed before mapping, it leads to a div to 0
    auto sbAddr = new ScrollbarAddress(this, _medusa);
    this->addressDock->setWidget(sbAddr);
    connect(sbAddr, SIGNAL(goTo(medusa::Address const&)), this, SLOT(goTo(medusa::Address const&)));

    this->actionOpen->setEnabled(false);
    this->actionLoad->setEnabled(false);
    this->actionGoto->setEnabled(true);
    this->actionClose->setEnabled(true);
    this->_documentOpened = true;
    this->setWindowTitle("Medusa - " + this->_fileName);

    addDisassemblyView(_medusa.GetDocument().GetStartAddress());

    return true;
  });
}

bool MainWindow::loadDocument()
{
  if (_documentOpened)
    return false;

  if (!_medusa.OpenDocument([&](
    medusa::Path& rDatabasePath,
    std::list<medusa::Medusa::Filter> const& rExtensionFilter
    )
  {
    QStringList Filters;
    for (auto itFlt = std::begin(rExtensionFilter), itEnd = std::end(rExtensionFilter); itFlt != itEnd; ++itFlt)
    {
      Filters.push_back(QString("%1 (*%2)").arg(std::get<0>(*itFlt).c_str(), std::get<1>(*itFlt).c_str()));
    }
    rDatabasePath = QFileDialog::getOpenFileName(this, "Select a saved database", QString(), Filters.join(";;")).toStdString();
    if (rDatabasePath.empty())
      return false;
    _fileName = QString::fromStdString(rDatabasePath.string());
    return true;
  }))
    return false;

  // Widgets initialisation must be called before file mapping... Except scrollbar address
  auto memAreaView = new MemoryAreaView(this, _medusa);
  memAreaView->Refresh();
  this->memAreaDock->setWidget(memAreaView);

  auto labelView = new LabelView(this, _medusa);
  labelView->Refresh();
  this->labelDock->setWidget(labelView);

  //medusa::Architecture::VSPType archs;
  //archs.push_back(architecture);
  //this->_medusa.Start(db->GetBinaryStream(), db, loader, archs, os);

  // FIXME If this is placed before mapping, it leads to a div to 0
  auto sbAddr = new ScrollbarAddress(this, _medusa);
  sbAddr->Refresh();
  this->addressDock->setWidget(sbAddr);

  this->actionOpen->setEnabled(false);
  this->actionLoad->setEnabled(false);
  this->actionGoto->setEnabled(true);
  this->actionClose->setEnabled(true);
  this->_documentOpened = true;
  this->setWindowTitle("Medusa - " + this->_fileName);

  addDisassemblyView(_medusa.GetDocument().GetStartAddress());

  connect(labelView,   SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
  connect(sbAddr,      SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
  connect(memAreaView, SIGNAL(goTo(medusa::Address const&)), this,                 SLOT(goTo(medusa::Address const&)));
  connect(this,        SIGNAL(lastAddressUpdated(medusa::Address const&)), sbAddr, SLOT(setCurrentAddress(medusa::Address const&)));

  return true;
}

bool MainWindow::saveDocument()
{
  return _medusa.GetDocument().Flush();
}

bool MainWindow::closeDocument()
{
  if (!_documentOpened)
    return false;

  int Result = QMessageBox::question(
    this, "Quit", "Do you wish to save the current document before quitting?",
    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
  switch (Result)
  {
  case QMessageBox::Yes:
    _medusa.GetDocument().Flush();
    break;

  case QMessageBox::No:
    break;

  default:
    return false;
  }

  this->_documentOpened = false;
  this->actionOpen->setEnabled(true);
  this->actionLoad->setEnabled(true);
  this->actionClose->setEnabled(false);
  this->actionGoto->setEnabled(false);

  if (!_medusa.CloseDocument())
    return false;

  //this->logEdit->clear();
  auto labelView = this->labelDock->widget();
  this->labelDock->setWidget(nullptr);
  delete labelView;
  
  auto memAreaDockview =  this->memAreaDock->widget();
  this->memAreaDock->setWidget(nullptr);
  delete memAreaDockview;
  
  auto BindingView = this->memAreaDock->widget();
  this->BindingDock->setWidget(nullptr);
  delete BindingView;

  int tabWidgetCount = this->tabWidget->count();
  for (int i = 0; i < tabWidgetCount; ++i)
  {
    auto curWidget = this->tabWidget->widget(i);
    this->tabWidget->removeTab(i);
    delete curWidget;
  }

  return true;
}

void MainWindow::appendLog(std::string const& msg)
{
  emit logAppended(QString::fromStdString(msg));
}

void MainWindow::addDisassemblyView(medusa::Address const& startAddr)
{
  auto disasmView = new DisassemblyView(this, &_medusa);
  connect(disasmView, SIGNAL(cursorAddressUpdated(medusa::Address const&)), this->addressDock->widget(), SLOT(setCurrentAddress(medusa::Address const&)));
  connect(disasmView, SIGNAL(cursorAddressUpdated(medusa::Address const&)), this, SLOT(setCurrentAddress(medusa::Address const&)));
  this->tabWidget->addTab(disasmView, QIcon(":/icons/view-disassembly.png"), "Disassembly (text)");
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
  this->tabWidget->addTab(semView, QIcon(":/icons/view-semantic.png"), QString("Semantic of function %1").arg(funcLbl));
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
  this->tabWidget->addTab(cfgView, QIcon(":/icons/view-graph.png"), QString("Graph of function %1").arg(funcLbl));
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
    int Reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to close this document?", QMessageBox::Yes | QMessageBox::No);

    if (Reply == QMessageBox::No)
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
  //https://qt-project.org/forums/viewthread/19083
  bool DoScroll = (logEdit->verticalScrollBar()->sliderPosition() == logEdit->verticalScrollBar()->maximum());
  logEdit->insertPlainText(msg);
  if (DoScroll)
    logEdit->verticalScrollBar()->setSliderPosition(logEdit->verticalScrollBar()->maximum());
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

void MainWindow::setCurrentAddress(medusa::Address const& addr)
{
  medusa::TOffset Off;
  QString OffStr = "";
  if (!_medusa.GetDocument().ConvertAddressToFileOffset(addr, Off))
    OffStr = "(unknown)";
  else
    OffStr.sprintf("0x%016x", Off);
  statusBar()->showMessage(QString("va: %1 \xE2\x86\x92 offset: %2").arg(QString::fromStdString(addr.ToString()), OffStr));
}

void MainWindow::closeEvent(QCloseEvent * event)
{
  medusa::UserConfiguration UserCfg;
  UserCfg.SetOption("ui_qt.geometry", medusa::Base64Encode(std::string(saveGeometry().constData(), saveGeometry().length())));
  UserCfg.SetOption("ui_qt.state", medusa::Base64Encode(std::string(saveState().constData(), saveState().length())));
  event->accept();
}
