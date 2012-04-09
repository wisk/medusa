#include "Proxy.hpp"

EventProxy::EventProxy(MainWindow * mainWnd) : _mainWnd(mainWnd)
{
}

bool EventProxy::OnDatabaseUpdated(void)
{
  _mainWnd->updateDisassemblyView();
  return true;
}

bool EventProxy::OnLabelAdded(LabelAdded const& labelAdded)
{
  _mainWnd->addLabel(labelAdded.GetLabel());
  return true;
}