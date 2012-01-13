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
  class Quit{};

  typedef boost::variant<Quit> EventType;

  virtual bool OnQuit(void)                         { return false; }
  virtual bool OnCellsUpdated(Address::List const&) { return true;  }
  virtual bool OnLogWritten(std::wstring const&)    { return true;  }

  bool operator()(EventType const& rEvent)
  {
    return boost::apply_visitor(*this, rEvent);
  }

  bool operator()(Quit const&)                                { return OnQuit();                              }
  bool operator()(Address::List const& rModifiedCellsAddress) { return OnCellsUpdated(rModifiedCellsAddress); }
  bool operator()(std::wstring const& rMessage)               { return OnLogWritten(rMessage);                }
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EVENT_HANDLER_
