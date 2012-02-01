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
#include "MedusaPrinterPool.hpp"
#include "Callback.hpp"
#include "medusa/log.hpp"

MEDUSA_NAMESPACE_USE

/* Static callback for medusa */

void	MainWindow::log(wchar_t const * text)
{
	if (MainWindow::_log != 0)
	{
		MainWindow::_log->insertPlainText(QString::fromWCharArray(text));
		MainWindow::_log->verticalScrollBar()->setValue(MainWindow::_log->verticalScrollBar()->maximum());
	}
}

QPlainTextEdit *	MainWindow::_log = 0;

/* Class */

MainWindow::MainWindow()
	: QMainWindow(), Ui::MainWindow(),
	_about(this),
	_openConfirmation(this),
	_loaderChooser(this, this->_medusa),
	_goto(this),
	_settingsDialog(this),
	_undoJumpView(),
	_documentOpened(false),
	_fileName(),
	_loader(&this->_editor, this->_medusa),
	_unLoader(&this->_editor, this->_medusa),
	_closeWindow(false),
	_openDocument(false),
	_medusa(),
	_selectedLoader(),
	_editor(),
	_editorTimer(this),
	_progressTimer(this)
{
	this->setupUi(this);

	MainWindow::_log = this->logEdit;
  medusa::Log::SetLog(medusaLog);

	this->_editor.setParent(this->tabDisassembly);
	this->verticalLayout_2->addWidget(&this->_editor);

	connect(&this->_editorTimer, SIGNAL(timeout()), this->_editor.viewport(), SLOT(update()));
	connect(&this->_loader, SIGNAL(finished()), this, SLOT(loader_finished()));
	connect(&this->_unLoader, SIGNAL(finished()), this, SLOT(unLoader_finished()));

	connect(this->dataList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(_on_label_clicked(QListWidgetItem *)));
	connect(this->codeList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(_on_label_clicked(QListWidgetItem *)));
	connect(this->stringList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(_on_label_clicked(QListWidgetItem *)));
	connect(this->importedList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(_on_label_clicked(QListWidgetItem *)));
	connect(this->exportedList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(_on_label_clicked(QListWidgetItem *)));

	this->_editorTimer.setInterval(500);
	this->_editorTimer.setSingleShot(false);

	this->_progressTimer.setInterval(500);
	this->_progressTimer.setSingleShot(false);

	this->progressBar->hide();

	this->_loader.moveToThread(&this->_loader);
	this->_unLoader.moveToThread(&this->_unLoader);

	this->restoreGeometry(Settings::instance().value(WINDOW_GEOMETRY, WINDOW_GEOMETRY_DEFAULT).toByteArray());
	this->restoreState(Settings::instance().value(WINDOW_LAYOUT, WINDOW_LAYOUT_DEFAULT).toByteArray());

	MedusaPrinterPool::setMainWindow(this);

	QAction * prev = this->_editor.undoJumpStack().createUndoAction(this);
	QAction * next = this->_editor.undoJumpStack().createRedoAction(this);

	prev->setIcon(QIcon(":images/prev.png"));
	next->setIcon(QIcon(":images/next.png"));

	prev->setShortcut(QKeySequence(Qt::Key_Backspace));

	this->toolBar->addAction(prev);
	this->toolBar->addAction(next);

	this->_undoJumpView.setStack(&this->_editor.undoJumpStack());
	this->undoWidget->setWidget(&this->_undoJumpView);
}

MainWindow::~MainWindow()
{
}

bool		MainWindow::openDocument()
{
	if (this->_loader.isRunning() || this->_unLoader.isRunning())
		return (false);

	this->_fileName = QFileDialog::getOpenFileName(this, tr("Select a file"));

	if (this->_fileName.isNull())
		return (false);

	// Opening file and loading module
	this->_medusa.Open(this->_fileName.toStdWString());
	this->_medusa.LoadModules(L".");

	medusaLog(QString("Opening %1\n").arg(this->_fileName).toStdWString().c_str());

	medusa::Loader::VectorSPtr const & loaders = this->_medusa.GetSupportedLoaders();

	// If no compatible loader was found
	if (loaders.empty())
	{
		QMessageBox::critical(this, tr("Loader error"), tr("There is no supported loader for this file"));
		this->closeDocument();
		return (false);
	}

	// Select archi
    medusa::Architecture::VectorSPtr const & archis = this->_medusa.GetArchitectures();

	// If no compatible archi was found
	if (archis.empty())
	{
		QMessageBox::critical(this, tr("Architecture error"), tr("There is no supported architecture for this file"));
		this->closeDocument();
		return (false);
	}

	medusa::Loader::SPtr loader;
	medusa::Architecture::SPtr architecture;

	if (!this->_loaderChooser.getSelection(loader, architecture))
	{
		this->closeDocument();
		return (false);
	}

	this->_selectedLoader = loader;

	this->statusbar->showMessage(tr("Interpreting executable format using ") + loader->GetName());
	loader->Map();

	this->statusbar->showMessage(tr("Disassembling ..."));

	try
	{
		this->_medusa.Disassemble(loader, architecture);
	}
	catch (...)
	{
		this->closeDocument();
		QMessageBox::critical(this, tr("Error"), tr("Error while disassembling"));
		return (false);
	}

	this->statusbar->showMessage(tr("Loading ..."));

	// Content
	Database & database = this->_medusa.GetDatabase();
	Database::TMemoryAreas & memoryAreas = database.GetMemoryAreas();
	Database::TMemoryAreas::const_iterator end = memoryAreas.end();
	int total = 0;

	for (Database::TMemoryAreas::const_iterator memoryArea = memoryAreas.begin();
		memoryArea != end; ++memoryArea)
		total += (*memoryArea)->GetSize();

	this->progressBar->setRange(0, total);
	this->progressBar->setValue(0);

	this->progressBar->show();

	this->_progressTimer.disconnect();
	this->connect(&this->_progressTimer, SIGNAL(timeout()), SLOT(progressTimer_timeout_unload()));

	this->_editor.viewport()->show();

	this->_progressTimer.start();
	this->_editorTimer.start();
	this->_loader.start();

	this->actionGoto->setEnabled(true);
	this->_documentOpened = true;
	this->setWindowTitle("Medusa - " + this->_fileName);

	return (true);
}

bool		MainWindow::closeDocument()
{
	if (this->_unLoader.isRunning())
		return (true);

	this->actionClose->setEnabled(false);
	this->actionGoto->setEnabled(false);

	this->progressBar->setRange(0, this->_editor.itemCount());
	this->progressBar->setValue(this->_editor.itemCount());

	this->progressBar->show();

	this->statusbar->showMessage(tr("Unloading project"));

	this->_log->clear();
	this->dataList->clear();
	this->codeList->clear();
	this->stringList->clear();
	this->importedList->clear();
	this->exportedList->clear();

	this->_progressTimer.disconnect();
	this->connect(&this->_progressTimer, SIGNAL(timeout()), SLOT(progressTimer_timeout_unload()));

	this->_editor.viewport()->hide();

	this->_unLoader.start();
	//this->_editorTimer.start();
	this->_progressTimer.start();

	return (true);
}

void		MainWindow::on_actionAbout_triggered()
{
	this->_about.exec();
}

void		MainWindow::on_actionOpen_triggered()
{
	if (this->_loader.isRunning() || this->_unLoader.isRunning())
		return;

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
	else
		this->openDocument();
}

void		MainWindow::on_actionClose_triggered()
{
	if (this->_loader.isRunning() || this->_unLoader.isRunning())
		return;

	this->closeDocument();
}

void		MainWindow::on_actionGoto_triggered()
{
	if (this->_goto.exec() == QDialog::Rejected)
		return;

	if (this->_goto.value() == -1)
		QMessageBox::warning(this, "Input error", "The input text is invalid");

	/*if (this->_documentOpened)
		this->_editor.goTo(this->_goto.value(), this->_goto.type());*/
}

void		MainWindow::on_actionSettings_triggered()
{
	this->_settingsDialog.exec();
}

void		MainWindow::_on_label_clicked(QListWidgetItem * item)
{
	medusa::Database & database = this->_medusa.GetDatabase();

	medusa::Address address = database.GetAddressFromLabelName(item->text().toStdString());
	this->_editor.goTo(MedusaEditor::Address(address.GetBase(), address.GetOffset()), QString("Go to %1").arg(item->text()));
}

void		MainWindow::loader_finished()
{
	this->_editorTimer.stop();
	this->_progressTimer.stop();
	this->progressBar->hide();
	this->_editor.viewport()->update();
	this->actionClose->setEnabled(true);
	this->statusbar->showMessage(tr("Finished"));

	// Loading Labels
	medusa::Database & database = this->_medusa.GetDatabase();
	medusa::Database::TLabelMap labelMap = database.GetLabels();
	medusa::Database::TLabelMap::const_iterator begin = labelMap.begin();

	while (begin != labelMap.end()) {
		switch (begin->right.GetType())
		{
		case medusa::Label::LabelData:
			this->dataList->addItem(QString::fromStdString(begin->right.GetName()));
			break;
		case medusa::Label::LabelCode:
			this->codeList->addItem(QString::fromStdString(begin->right.GetName()));
			break;
		case medusa::Label::LabelString:
			this->stringList->addItem(QString::fromStdString(begin->right.GetName()));
			break;
		default:
			break;
		}
		if (begin->right.GetType() & medusa::Label::LabelImported)
			this->importedList->addItem(QString::fromStdString(begin->right.GetName()));
		else if (begin->right.GetType() & medusa::Label::LabelExported)
			this->exportedList->addItem(QString::fromStdString(begin->right.GetName()));

		++begin;
	}

	medusa::Address address = this->_selectedLoader->GetEntryPoint();

	this->_editor.goTo(QPair<medusa::TBase, medusa::TOffset>(address.GetBase(), address.GetOffset()), "Go to entry point");
}

void		MainWindow::unLoader_finished()
{
	this->_editorTimer.stop();
	this->_progressTimer.stop();

	//this->_editor.viewport()->show();
	this->_editor.viewport()->update();

	this->_documentOpened = false;
	this->setWindowTitle("Medusa");

	this->statusbar->showMessage(tr("Finished"));

	this->progressBar->hide();

	if (this->_closeWindow)
		this->close();

	if (this->_openDocument)
	{
		this->_openDocument = false;
		this->openDocument();
	}
}

void		MainWindow::progressTimer_timeout_unload()
{
	this->progressBar->setValue(this->_editor.itemCount());
}

void		MainWindow::closeEvent(QCloseEvent * event)
{
	if (this->_loader.isRunning() || this->_unLoader.isRunning())
	{
		event->ignore();
		return;
	}

	// if a document is currently opent
	if (this->_documentOpened)
	{
		// Close it, but prevent window from closing while unloading
		if (this->closeDocument())
			this->_closeWindow = true;
		event->ignore();
	}
	else
	{
		Settings::instance().setValue(WINDOW_LAYOUT, this->saveState());
		Settings::instance().setValue(WINDOW_GEOMETRY, this->saveGeometry());
		event->accept();
	}
}
