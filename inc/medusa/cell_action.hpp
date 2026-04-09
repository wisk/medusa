#ifndef MEDUSA_CELL_ACTION_HPP
#define MEDUSA_CELL_ACTION_HPP

#include <string>
#include <map>

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include "medusa/cell.hpp"
#include "medusa/architecture.hpp"
#include "medusa/medusa.hpp"
#include "medusa/disassembly_view.hpp"

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Action
{
public:
  using SPType = std::shared_ptr<Action>;
  using SPTypeList = std::list<SPType>;

  using CreateType = SPType (*)(Medusa& rCore, FullDisassemblyView* pView);
  using MapType = std::map<char const*, CreateType>;

  using RangeAddress = std::pair<Address, Address>;

  static SPType        Create(Medusa& rCore, FullDisassemblyView* pView);
  static char const* GetBindingName();

  virtual ~Action() = default;

  virtual std::string GetName() const = 0;
  virtual std::string GetDescription() const = 0;
  virtual std::string GetIconName() const = 0;
  virtual bool        IsCompatible() const = 0;
  virtual void        Do() = 0;

  static MapType  GetMap();
  static SPTypeList GetSpecificActions(Medusa& rCore, FullDisassemblyView* pView, Address const& rAddress);

protected:
  Action(Medusa& rCore, FullDisassemblyView* pView) : m_rCore(rCore), m_pView(pView) {}

  Medusa& m_rCore;
  FullDisassemblyView* m_pView;

private:
  Action(Action const&);
  Action& operator=(Action const&);
};

MEDUSA_NAMESPACE_END

#endif //!MEDUSA_CELL_ACTION_HPP
