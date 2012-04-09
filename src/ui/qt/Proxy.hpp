#ifndef __MEDUSA_PROXY_HPP__
#define __MEDUSA_PROXY_HPP__

#include "MainWindow.hpp"
#include <medusa/event_handler.hpp>

class EventProxy : public medusa::EventHandler
{
public:
  EventProxy(MainWindow * mainWnd);
  ~EventProxy(void);

  virtual bool OnDatabaseUpdated(void);
  virtual bool OnLabelAdded(LabelAdded const& labelAdded);

private:
  MainWindow * _mainWnd;
};

#endif // !__MEDUSA_PROXY_HPP__