#ifndef MEDUSA_CELL_ACTION_HPP
#define MEDUSA_CELL_ACTION_HPP

#include <string>
#include <map>

#include <boost/bind.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include "medusa/cell.hpp"
#include "medusa/architecture.hpp"
#include "medusa/medusa.hpp"
#include "medusa/disassembly_view.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Action
{
public:
  typedef std::shared_ptr<Action> SPtr;
  typedef std::list<SPtr> SPtrList;

  typedef SPtr (*CreateType)(Medusa& rCore, FullDisassemblyView* pView);
  typedef std::map<char const*, CreateType> MapType;

  typedef std::pair<Address, Address> RangeAddress;

  static SPtr        Create(Medusa& rCore, FullDisassemblyView* pView);
  static char const* GetBindingName(void);

  virtual ~Action(void) {}

  virtual std::string GetName(void) const = 0;
  virtual std::string GetDescription(void) const = 0;
  virtual std::string GetIconName(void) const = 0;
  virtual bool        IsCompatible(void) const = 0;
  virtual void        Do(void) = 0;

  static MapType  GetMap(void);
  static SPtrList GetSpecificActions(Medusa& rCore, FullDisassemblyView* pView, Address const& rAddress);

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
