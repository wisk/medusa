#include "Proxy.hpp"

EventProxy::EventProxy(MainWindow * mainWnd) : _mainWnd(mainWnd)
{
}

bool EventProxy::OnDatabaseUpdated(void)
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