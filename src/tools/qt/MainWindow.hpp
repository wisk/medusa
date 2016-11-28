#ifndef QMEDUSA_MAIN_WINDOW_HPP
#define QMEDUSA_MAIN_WINDOW_HPP

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
# include "LabelView.hpp"
# include "ScrollbarAddress.hpp"
# include "MemoryAreaView.hpp"

# include "DisassemblyView.hpp"
# include "SemanticView.hpp"
# include "GraphView.hpp"
# include "GraphScene.hpp"
# include "ControlFlowGraphView.hpp"
# include "ControlFlowGraphScene.hpp"
# include "BindingView.hpp"

# include "Goto.hpp"
# include "SettingsDialog.hpp"

class QCloseEvent;

Q_DECLARE_METATYPE(medusa::Address)

class MainWindow : public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow(QString const& rFilePath = "", QString const& rDbPath = "");
  ~MainWindow();

public:
  bool        openDocument();
  bool        loadDocument();
  bool        saveDocument();
  bool        closeDocument();

  void        appendLog(std::string const & msg);

public slots:
  void        addDisassemblyView(medusa::Address const& startAddr);
  void        addSemanticView(medusa::Address const& funcAddr);
  void        addGraphView(medusa::Address const& rMcAddr);
  void        addControlFlowGraphView(medusa::Address const& funcAddr);

  void        on_actionAbout_triggered();
  void        on_actionOpen_triggered();
  void        on_actionLoad_triggered();
  void        on_actionSave_triggered();
  void        on_actionClose_triggered();
  void        on_actionGoto_triggered();
  void        on_actionDisassembly_triggered();
  void        on_actionSettings_triggered();
  void        on_tabWidget_tabCloseRequested(int index);
  void        onLogMessageAppended(QString const & msg);

  void        goTo(medusa::Address const& addr);
  void        setCurrentAddress(medusa::Address const& addr);

signals:
  void        DisassemblyViewAdded(medusa::Address const& startAddr);
  void        SemanticViewAdded(medusa::Address const& funcAddr);
  void        GraphViewAdded(medusa::Address const& rMcAddr);
  void        ControlFlowGraphViewAdded(medusa::Address const& funcAddr);

  void        logAppended(QString const & msg);
  void        addNewView(medusa::Address const& addr);

  void        lastAddressUpdated(medusa::Address const& addr);

protected:
  void        closeEvent(QCloseEvent * event);

private:
  // Dialog
  About                     _about;
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
};

#endif // !QMEDUSA_MAIN_WINDOW_HPP
