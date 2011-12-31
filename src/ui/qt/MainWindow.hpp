#ifndef __MAIN_WINDOW_H__
# define __MAIN_WINDOW_H__

# include <QMainWindow>
# include <QString>
# include <QTextDocument>
# include <QTextTable>
# include <QTimer>
# include <QUndoView>
# include <medusa/medusa.hpp>
# include "ui_MainWindow.h"
# include "About.hpp"
# include "OpenConfirmation.hpp"
# include "LoaderChooser.hpp"
# include "MedusaEditor.hpp"
# include "Loader.hpp"
# include "UnLoader.hpp"
# include "Goto.hpp"
# include "SettingsDialog.hpp"

class QCloseEvent;

class MainWindow : public QMainWindow, public Ui::MainWindow
{

	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

public:
	bool				openDocument();
	bool				closeDocument();

public slots:
	void				on_actionAbout_triggered();
	void				on_actionOpen_triggered();
	void				on_actionClose_triggered();
	void				on_actionGoto_triggered();
	void				on_actionSettings_triggered();

	void				loader_finished();
	void				unLoader_finished();
	void				progressTimer_timeout_unload();

protected:
	void				closeEvent(QCloseEvent * event);

private:
	// Dialog
	About				_about;
	OpenConfirmation	_openConfirmation;
	LoaderChooser		_loaderChooser;
	Goto				_goto;
	SettingsDialog		_settingsDialog;
	

	// UI
	QUndoView			_undoJumpView;

	// Data
	bool				_documentOpened;
	QString				_fileName;
	Loader				_loader;
	UnLoader			_unLoader;
	bool				_closeWindow;
	bool				_openDocument;

	// Core
	medusa::Medusa		_medusa;
	medusa::Loader::Ptr	_selectedLoader;

	// Editor
	MedusaEditor		_editor;
	QTimer				_editorTimer;
	QTimer				_progressTimer;

};

#endif // !__MAIN_WINDOW_H__