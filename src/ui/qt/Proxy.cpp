#include "Proxy.hpp"

#include <QApplication>

EventProxy::EventProxy(MainWindow * mainWnd) : _mainWnd(mainWnd)
{
}

bool EventProxy::OnDocumentUpdated(void)
{
  _mainWnd->updateDisassemblyView();
  return true;
}

bool EventProxy::OnLabelUpdated(LabelUpdated const& labelUpdated)
{
  switch (labelUpdated.GetType())
  {
  case medusa::EventHandler::LabelUpdated::Add:
    _mainWnd->addLabel(labelUpdated.GetLabel());
    return true;
  case medusa::EventHandler::LabelUpdated::Remove:
    _mainWnd->removeLabel(labelUpdated.GetLabel());
    return true;
  default:
    return false;
  }
}

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