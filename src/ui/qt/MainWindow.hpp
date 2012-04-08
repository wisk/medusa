#ifndef __MAIN_WINDOW_H__
# define __MAIN_WINDOW_H__

# include <QMainWindow>
# include <QString>
# include <QTextDocument>
# include <QTextTable>
# include <QTimer>
# include <QUndoView>
# include <QPlainTextEdit>
# include <QListWidgetItem>
# include <medusa/medusa.hpp>
# include "ui_MainWindow.h"
# include "About.hpp"
# include "OpenConfirmation.hpp"
# include "LoaderChooser.hpp"
# include "DisassemblyView.hpp"
# include "Goto.hpp"
# include "SettingsDialog.hpp"

class QCloseEvent;

Q_DECLARE_METATYPE(medusa::Address)

class MainWindow : public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT

public:
  static void      log(wchar_t const * text);

public:
  MainWindow();
  ~MainWindow();

public:
  bool        openDocument();
  bool        closeDocument();

  void        updateDisassemblyView(void);

public slots:
  void        on_actionAbout_triggered();
  void        on_actionOpen_triggered();
  void        on_actionClose_triggered();
  void        on_actionGoto_triggered();
  void        on_actionSettings_triggered();
  void        _on_label_clicked(QListWidgetItem * item);

signals:
  void        disassemblyListingUpdated(void);

protected:
  void        closeEvent(QCloseEvent * event);

private:
  // Dialog
  About                    _about;
  OpenConfirmation         _openConfirmation;
  LoaderChooser            _loaderChooser;
  Goto                     _goto;
  SettingsDialog           _settingsDialog;

  // UI
  QUndoView                _undoJumpView;

  // Data
  bool                     _documentOpened;
  QString                  _fileName;
  bool                     _closeWindow;
  bool                     _openDocument;

  // Core
  medusa::Medusa           _medusa;
  medusa::Loader::SPtr     _selectedLoader;

  // Disassembly
  DisassemblyView          _disasmView;

  // Other
  static QPlainTextEdit *  _log;
};

#endif // !__MAIN_WINDOW_H__