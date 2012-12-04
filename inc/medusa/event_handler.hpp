#ifndef _MEDUSA_EVENT_HANDLER_
#define _MEDUSA_EVENT_HANDLER_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/address.hpp"

#include "medusa/label.hpp"

#include <string>
#include <boost/variant.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT EventHandler : public boost::static_visitor<bool>
{
public:
  class Quit {};
  class DatabaseUpdated {};

  class LabelUpdated
  {
  public:
    enum UpdateType
    {
      Add,
      Remove
    };
    LabelUpdated(Label const& rLbl, UpdateType Type) : m_Lbl(rLbl), m_Type(Type) {}
    Label const& GetLabel(void) const { return m_Lbl; }
    UpdateType GetType(void) const { return m_Type; }
  private:
    Label m_Lbl;
    UpdateType m_Type;
  };

  typedef boost::variant<Quit, DatabaseUpdated, LabelUpdated> EventType;

  virtual bool OnQuit(void)                    { return false; }
  virtual bool OnDatabaseUpdated(void)         { return true;  }
  virtual bool OnLabelUpdated(LabelUpdated const&) { return true;  }

  bool operator()(EventType const& rEvent)
  {
    return boost::apply_visitor(*this, rEvent);
  }

  bool operator()(Quit const&)                 { return OnQuit();                }
  bool operator()(DatabaseUpdated const&)      { return OnDatabaseUpdated();     }
  bool operator()(LabelUpdated const& rLblAdded) { return OnLabelUpdated(rLblAdded); }
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EVENT_HANDLER_