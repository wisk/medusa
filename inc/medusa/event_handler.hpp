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

  class UpdatedCell
  {
  public:
    UpdatedCell(Address::List const& rModifiedAddresses)
      : m_rModifiediedAddresses(rModifiedAddresses) {}

    Address::List const& GetModifiedAddresses(void) const
    { return m_rModifiediedAddresses; }

  private:
    Address::List m_rModifiediedAddresses;
  };

  typedef boost::variant<Quit, UpdatedCell> EventType;

  virtual bool OnQuit(void)                       { return false; }
  virtual bool OnCellUpdated(UpdatedCell const&)  { return true;  }

  bool operator()(EventType const& rEvent)
  {
    return boost::apply_visitor(*this, rEvent);
  }

  bool operator()(Quit const&)                     { return OnQuit();                    }
  bool operator()(UpdatedCell const& rUpdatedCell) { return OnCellUpdated(rUpdatedCell); }
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EVENT_HANDLER_