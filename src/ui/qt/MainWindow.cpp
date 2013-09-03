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
#include "Settings.hpp"
#include "Proxy.hpp"

#include "medusa/log.hpp"

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
  , _selectedLoader()
{
  this->setupUi(this);

  medusa::Log::SetLog(boost::bind(&MainWindow::appendLog, this, _1));

  this->tabWidget->setTabsClosable(true);

  this->restoreGeometry(Settings::instance().value(WINDOW_GEOMETRY, WINDOW_GEOMETRY_DEFAULT).toByteArray());
  this->restoreState(Settings::instance().value(WINDOW_LAYOUT, WINDOW_LAYOUT_DEFAULT).toByteArray());

  connect(this->tabWidget,       SIGNAL(tabCloseRequested(int)),               this, SLOT(on_tabWidget_tabCloseRequested(int)));
  connect(this,                  SIGNAL(logAppended(QString const &)),         this, SLOT(onLogMessageAppended(QString const &)));
}

MainWindow::~MainWindow()
{
}

bool MainWindow::openDocument()
{
  try
  {
    _fileName = QFileDialog::getOpenFileName(this, tr("Select a file"));

    if (_fileName.isNull())
      return false;

    // Opening file and loading module
    _medusa.Open(this->_fileName.toStdWString());
    _medusa.LoadModules(L".");

    emit logAppended(QString("Opening %1\n").arg(this->_fileName));

    medusa::Loader::VectorSharedPtr const & loaders = this->_medusa.GetSupportedLoaders();

    // If no compatible loader was found
    if (loaders.empty())
    {
      QMessageBox::critical(this, tr("Loader error"), tr("There is no supported loader for this file"));
      this->closeDocument();
      return false;
    }

    // Select arch
    medusa::Architecture::VectorSharedPtr const & archis = this->_medusa.GetAvailableArchitectures();

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

    LoaderChooser lc(this, _medusa);
    if (!lc.getSelection(loader, architecture, os))
    {
      this->closeDocument();
      return false;
    }

    this->_selectedLoader = loader;
    _medusa.RegisterArchitecture(architecture);

    this->statusbar->showMessage(tr("Interpreting executable format using ") + QString::fromStdString(loader->GetName()));
    loader->Map();

    auto labelView = new LabelView(this, _medusa);
    this->infoDock->setWidget(labelView);

    //auto sbAddr = new ScrollbarAddress(this, _medusa);
    //this->addressDock->setWidget(sbAddr);

    this->_medusa.SetOperatingSystem(os);
    this->_medusa.StartAsync(loader, architecture, os);

    this->actionGoto->setEnabled(true);
    this->_documentOpened = true;
    this->setWindowTitle("Medusa - " + this->_fileName);

    addDisassemblyView(loader->GetEntryPoint());

    connect(labelView, SIGNAL(goTo(medusa::Address const&)), this, SLOT(goTo(medusa::Address const&)));

  }
  catch (medusa::Exception const& e)
  {
    QMessageBox::critical(this, "Error", QString::fromStdWString(e.What()));
    this->closeDocument();
    return false;
  }

  return true;
}

bool MainWindow::closeDocument()
{
  this->_documentOpened = false;
  this->actionClose->setEnabled(false);
  this->actionGoto->setEnabled(false);

  this->logEdit->clear();
  auto labelView = this->infoDock->widget();
  disconnect(labelView, SIGNAL(goTo(medusa::Address const&)), this, SLOT(goTo(medusa::Address const&)));
  this->infoDock->setWidget(nullptr);
  delete labelView;

  int tabWidgetCount = this->tabWidget->count();
  for (int i = 0; i < tabWidgetCount; ++i)
  {
    auto curWidget = this->tabWidget->widget(i);
    this->tabWidget->removeTab(i);
    delete curWidget;
  }

  _medusa.Close();

  return true;
}

void MainWindow::appendLog(std::wstring const& msg)
{
  emit logAppended(QString::fromStdWString(msg));
}

void MainWindow::addDisassemblyView(medusa::Address const& startAddr)
{
  if (_medusa.IsOpened() == false)
    return;
  auto disasmView = new DisassemblyView(this, &_medusa);
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
    funcLbl = QString::fromStdString(lbl.GetName());

  auto semView = new SemanticView(this, _medusa, funcAddr);
  this->tabWidget->addTab(semView, QString("Semantic of function %1").arg(funcLbl));
}

void MainWindow::addControlFlowGraphView(medusa::Address const& funcAddr)
{
  auto func = _medusa.GetMultiCell(funcAddr);
  if (func == nullptr || func->GetType() != medusa::MultiCell::FunctionType)
    return;

  auto lbl = _medusa.GetDocument().GetLabelFromAddress(funcAddr);
  QString funcLbl = QString::fromStdString(funcAddr.ToString());
  if (lbl.GetType() != medusa::Label::Unknown)
    funcLbl = QString::fromStdString(lbl.GetName());

  auto cfgView = new ControlFlowGraphView(this);
  auto cfgScene = new ControlFlowGraphScene(this->tabWidget, _medusa, *static_cast<medusa::Function const*>(func));
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
  if (_medusa.IsOpened() == false)
    return;
  addDisassemblyView(_selectedLoader->GetEntryPoint());
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
    QMessageBox::warning(this, "Invalid address", "This address looks invalid");
}

void MainWindow::closeEvent(QCloseEvent * event)
{
  Settings::instance().setValue(WINDOW_LAYOUT, this->saveState());
  Settings::instance().setValue(WINDOW_GEOMETRY, this->saveGeometry());
  event->accept();
}