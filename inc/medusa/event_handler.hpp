#ifndef MEDUSA_EVENT_HANDLER_HPP
#define MEDUSA_EVENT_HANDLER_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/address.hpp"

#include "medusa/label.hpp"

#include <string>
#include <variant>

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT EventHandler
{
public:
  class Quit {};
  class DocumentUpdated {};

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

  using EventType = std::variant<Quit, DocumentUpdated, LabelUpdated>;

  virtual bool OnQuit(void)                        { return false; }
  virtual bool OnDocumentUpdated(void)             { return true;  }
  virtual bool OnLabelUpdated(LabelUpdated const&) { return true;  }

  bool operator()(EventType const& rEvent)
  {
    return std::visit(*this, rEvent);
  }

  bool operator()(Quit const&)                   { return OnQuit();                }
  bool operator()(DocumentUpdated const&)        { return OnDocumentUpdated();     }
  bool operator()(LabelUpdated const& rLblAdded) { return OnLabelUpdated(rLblAdded); }
};

class DocumentEvent
{
public:
  enum Type
  {
  };
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EVENT_HANDLER_HPP