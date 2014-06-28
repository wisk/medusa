#ifndef _MEDUSA_CELL_ACTION_
#define _MEDUSA_CELL_ACTION_

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

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Action
{
public:
  typedef std::shared_ptr<Action> SPtr;
  typedef std::list<SPtr> SPtrList;

  typedef SPtr (*CreateType)(Medusa& rCore);
  typedef std::map<char const*, CreateType> MapType;

  typedef std::pair<Address, Address> RangeAddress;

  static SPtr        Create(Medusa& rCore);
  static char const* GetBindingName(void);

  virtual ~Action(void) {}

  virtual std::string GetName(void) const = 0;
  virtual std::string GetDescription(void) const = 0;
  virtual std::string GetIconName(void) const = 0;
  virtual bool        IsCompatible(RangeAddress const& rRangeAddress) const = 0;
  virtual void        Do(RangeAddress const& rRangeAddress) = 0;

  static MapType& GetMap(void);

protected:
  Action(Medusa& rCore) : m_rCore(rCore) {}

  Medusa& m_rCore;

private:
  Action(Action const&);
  Action& operator=(Action const&);
};

MEDUSA_NAMESPACE_END

#endif //!_MEDUSA_CELL_ACTION_
