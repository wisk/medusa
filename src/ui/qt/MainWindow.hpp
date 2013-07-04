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
  void        addLabel(medusa::Label const& label);
  void        removeLabel(medusa::Label const& label);

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
  void        _on_label_clicked(QListWidgetItem * item);
  void        onLogMessageAppended(QString const & msg);
  void        onLabelAdded(medusa::Label const& label);
  void        onLabelRemoved(medusa::Label const& label);

signals:
  void        DisassemblyViewAdded(medusa::Address const& startAddr);
  void        SemanticViewAdded(medusa::Address const& funcAddr);
  void        ControlFlowGraphViewAdded(medusa::Address const& funcAddr);

  void        disassemblyListingUpdated(void);
  void        logAppended(QString const & msg);
  void        labelAdded(medusa::Label const& label);
  void        labelRemoved(medusa::Label const& label);
  void        addNewView(medusa::Address const& addr);

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
  bool                      _documentOpened;
  QString                   _fileName;
  bool                      _closeWindow;
  bool                      _openDocument;

  // Core
  medusa::Medusa            _medusa;
  medusa::Loader::SharedPtr _selectedLoader;

  // Disassembly
  DisassemblyView           _disasmView;
};

#endif // !__MAIN_WINDOW_H__