#ifndef _MEDUSA_EVENT_HANDLER_
#define _MEDUSA_EVENT_HANDLER_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/address.hpp"

#include <string>
#include <boost/variant.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT EventHandler : public boost::static_visitor<bool>
{
public:
  class Quit {};
  class DatabaseUpdated {};

  typedef boost::variant<Quit, DatabaseUpdated> EventType;

  virtual bool OnQuit(void)             { return false; }
  virtual bool OnDatabaseUpdated(void)  { return true;  }

  bool operator()(EventType const& rEvent)
  {
    return boost::apply_visitor(*this, rEvent);
  }

  bool operator()(Quit const&)            { return OnQuit();            }
  bool operator()(DatabaseUpdated const&) { return OnDatabaseUpdated(); }
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EVENT_HANDLER_