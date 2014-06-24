#include "Proxy.hpp"

#include "CommentDialog.hpp"
#include "LabelDialog.hpp"

#include <QApplication>

void AddDisassemblyViewAction::Do(medusa::Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addDisassemblyView(rAddrList.front());
}

void AddSemanticViewAction::Do(medusa::Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addSemanticView(rAddrList.front());
}

void AddControlFlowGraphViewAction::Do(medusa::Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addControlFlowGraphView(rAddrList.front());
}

void ShowCommentDialog::Do(medusa::Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;
  CommentDialog CmtDlg(qApp->activeWindow(), m_rCore, rAddrList.front());
  CmtDlg.exec();
}

void ShowLabelDialog::Do(medusa::Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;
  LabelDialog LblDlg(qApp->activeWindow(), m_rCore, rAddrList.front());
  LblDlg.exec();
}