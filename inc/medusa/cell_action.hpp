#ifndef _MEDUSA_CELL_ACTION_
#define _MEDUSA_CELL_ACTION_

#include <string>
#include <boost/bind.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include "medusa/cell.hpp"
#include "medusa/architecture.hpp"
#include "medusa/medusa.hpp"

MEDUSA_NAMESPACE_BEGIN

// TODO: redesign action to be more generic (cell / multicell / ...)
class Medusa_EXPORT CellAction
{
public:
  typedef CellAction* Ptr;
  typedef std::list<Ptr> PtrList;

  virtual std::string GetName(void)                   const { return "No name";             }
  virtual std::string GetDescription(void)            const { return "No description";      }
  virtual std::string GetIconName(void)               const { return "";                    }
  virtual bool        IsCompatible(Cell const& rCell) const { return false;                 }
  virtual void        Do(Medusa& rCore, Address::List const& rAddrList) { }

  static void GetCellActionBuilders(Medusa const& rCore, Address const& rAddress, PtrList& rActList);
};

class MultiCellAction
{
};

MEDUSA_NAMESPACE_END

#endif //!_MEDUSA_CELL_ACTION_
