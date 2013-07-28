#include "Proxy.hpp"

#include <QApplication>

void AddDisassemblyViewAction::Do(medusa::Medusa& rCore, medusa::Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addDisassemblyView(rAddrList.front());
}

void AddSemanticViewAction::Do(medusa::Medusa& rCore, medusa::Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addSemanticView(rAddrList.front());
}

void AddControlFlowGraphViewAction::Do(medusa::Medusa& rCore, medusa::Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;
  auto mainWin = dynamic_cast<MainWindow*>(qApp->activeWindow());
  if (mainWin == nullptr)
    return;
  mainWin->addControlFlowGraphView(rAddrList.front());
}