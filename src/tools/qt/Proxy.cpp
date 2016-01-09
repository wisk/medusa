#include "Proxy.hpp"

#include "CommentDialog.hpp"
#include "LabelDialog.hpp"

#include <QApplication>

void AddDisassemblyViewAction::Do(void)
{
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addDisassemblyView(m_pView->GetCursorAddress());
}

void AddSemanticViewAction::Do(void)
{
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addSemanticView(m_pView->GetCursorAddress());
}

void AddGraphViewAction::Do(void)
{
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addGraphView(m_pView->GetCursorAddress());
}

void AddControlFlowGraphViewAction::Do(void)
{
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addControlFlowGraphView(m_pView->GetCursorAddress());
}

void ShowCommentDialog::Do(void)
{
  CommentDialog CmtDlg(qApp->activeWindow(), m_rCore, m_pView->GetCursorAddress());
  CmtDlg.exec();
}

void ShowLabelDialog::Do(void)
{
  LabelDialog LblDlg(qApp->activeWindow(), m_rCore, m_pView->GetCursorAddress());
  LblDlg.exec();
}

void AddUiActions(medusa::Action::MapType& rActions)
{
  rActions[AddDisassemblyViewAction::GetBindingName()]      = &AddDisassemblyViewAction::Create;
  rActions[AddSemanticViewAction::GetBindingName()]         = &AddSemanticViewAction::Create;
  rActions[AddGraphViewAction::GetBindingName()]            = &AddGraphViewAction::Create;
  rActions[AddControlFlowGraphViewAction::GetBindingName()] = &AddControlFlowGraphViewAction::Create;
  rActions[ShowCommentDialog::GetBindingName()]             = &ShowCommentDialog::Create;
  rActions[ShowLabelDialog::GetBindingName()]               = &ShowLabelDialog::Create;
}