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

  class LabelAdded
  {
  public:
    LabelAdded(Label const& rLbl) : m_Lbl(rLbl) {}
    Label const& GetLabel(void) const { return m_Lbl; }
  private:
    Label m_Lbl;
  };

  typedef boost::variant<Quit, DatabaseUpdated, LabelAdded> EventType;

  virtual bool OnQuit(void)                    { return false; }
  virtual bool OnDatabaseUpdated(void)         { return true;  }
  virtual bool OnLabelAdded(LabelAdded const&) { return true;  }

  bool operator()(EventType const& rEvent)
  {
    return boost::apply_visitor(*this, rEvent);
  }

  bool operator()(Quit const&)                 { return OnQuit();                }
  bool operator()(DatabaseUpdated const&)      { return OnDatabaseUpdated();     }
  bool operator()(LabelAdded const& rLblAdded) { return OnLabelAdded(rLblAdded); }
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EVENT_HANDLER_