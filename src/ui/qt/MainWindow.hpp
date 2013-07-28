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
# include "LabelView.hpp"

# include "DisassemblyView.hpp"
# include "SemanticView.hpp"
# include "ControlFlowGraphView.hpp"
# include "ControlFlowGraphScene.hpp"

# include "Goto.hpp"
# include "SettingsDialog.hpp"

class QCloseEvent;

Q_DECLARE_METATYPE(medusa::Address)

class MainWindow : public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

public:
  bool        openDocument();
  bool        closeDocument();

  void        updateDisassemblyView(void);
  void        appendLog(std::wstring const & msg);

public slots:
  void        addDisassemblyView(medusa::Address const& startAddr);
  void        addSemanticView(medusa::Address const& funcAddr);
  void        addControlFlowGraphView(medusa::Address const& funcAddr);

  void        on_actionAbout_triggered();
  void        on_actionOpen_triggered();
  void        on_actionClose_triggered();
  void        on_actionGoto_triggered();
  void        on_actionDisassembly_triggered();
  void        on_actionSettings_triggered();
  void        on_tabWidget_tabCloseRequested(int index);
  void        onLogMessageAppended(QString const & msg);
  void        onStatusbarUpdated();

  void        goTo(medusa::Address const& addr);

signals:
  void        DisassemblyViewAdded(medusa::Address const& startAddr);
  void        SemanticViewAdded(medusa::Address const& funcAddr);
  void        ControlFlowGraphViewAdded(medusa::Address const& funcAddr);

  void        disassemblyListingUpdated(void);
  void        logAppended(QString const & msg);
  void        addNewView(medusa::Address const& addr);
  void        updateStatusbar();

protected:
  void        closeEvent(QCloseEvent * event);

private:
  // Dialog
  About                     _about;
  OpenConfirmation          _openConfirmation;
  LoaderChooser             _loaderChooser;
  Goto                      _goto;
  SettingsDialog            _settingsDialog;

  // UI
  QUndoView                 _undoJumpView;

  // Data
  QString                   _fileName;
  bool                      _documentOpened;
  bool                      _closeWindow;
  bool                      _openDocument;

  // Core
  medusa::Medusa            _medusa;
  medusa::Loader::SharedPtr _selectedLoader;
};

#endif // !__MAIN_WINDOW_H__