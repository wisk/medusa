#include "Proxy.hpp"

#include "CommentDialog.hpp"
#include "LabelDialog.hpp"

#include <QApplication>

void AddDisassemblyViewAction::Do(medusa::Action::RangeAddress const& rRangeAddress)
{
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addDisassemblyView(rRangeAddress.second);
}

void AddSemanticViewAction::Do(medusa::Action::RangeAddress const& rRangeAddress)
{
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addSemanticView(rRangeAddress.second);
}

void AddControlFlowGraphViewAction::Do(medusa::Action::RangeAddress const& rRangeAddress)
{
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addControlFlowGraphView(rRangeAddress.second);
}

void ShowCommentDialog::Do(medusa::Action::RangeAddress const& rRangeAddress)
{
  CommentDialog CmtDlg(qApp->activeWindow(), m_rCore, rRangeAddress.second);
  CmtDlg.exec();
}

void ShowLabelDialog::Do(medusa::Action::RangeAddress const& rRangeAddress)
{
  LabelDialog LblDlg(qApp->activeWindow(), m_rCore, rRangeAddress.second);
  LblDlg.exec();
}

void AddUiActions(medusa::Action::MapType& rActions)
{
  rActions[AddDisassemblyViewAction::GetBindingName()] = &AddDisassemblyViewAction::Create;
  rActions[AddSemanticViewAction::GetBindingName()] = &AddSemanticViewAction::Create;
  rActions[AddControlFlowGraphViewAction::GetBindingName()] = &AddControlFlowGraphViewAction::Create;
  rActions[ShowCommentDialog::GetBindingName()] = &ShowCommentDialog::Create;
  rActions[ShowLabelDialog::GetBindingName()] = &ShowLabelDialog::Create;
}