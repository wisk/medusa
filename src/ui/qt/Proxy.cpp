#include "Proxy.hpp"

EventProxy::EventProxy(MainWindow * mainWnd) : _mainWnd(mainWnd)
{
}

bool EventProxy::OnDatabaseUpdated(void)
{
  _mainWnd->updateDisassemblyView();
  return true;
}