#ifndef _MEDUSA_CELL_ACTION_
#define _MEDUSA_CELL_ACTION_

#include <string>
#include <boost/bind.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include "medusa/cell.hpp"
#include "medusa/medusa.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT CellAction
{
public:
  typedef CellAction* Ptr;
  typedef std::list<Ptr> PtrList;

  virtual std::string GetName(void)                   const { return "No name";             }
  virtual std::string GetDescription(void)            const { return "No description";      }
  virtual bool        IsCompatible(Cell const& rCell) const { return false;                 }
  virtual void        Do(Medusa& rCore, Address::List const& rAddrList) { }

  static void GetCellActionBuilders(PtrList& rActList);
  static void GetCellActionBuilders(Cell const& rCell, PtrList& rActList);
};

class CellAction_Undefine : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Undefine"; }

  virtual std::string GetDescription(void) const
  { return "This option converts the selected item to byte"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != CellData::ValueType || rCell.GetLength() != 1; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_ChangeValueSize : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Change value size"; }

  virtual std::string GetDescription(void) const
  { return "This option allows to change the size of a value (byte, word, dword, qword)"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() == CellData::ValueType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_Disassemble : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Disassemble"; }

  virtual std::string GetDescription(void) const
  { return "Disassemble using the default architecture module"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != CellData::InstructionType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_Analyze : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Analyze"; }

  virtual std::string GetDescription(void) const
  { return "Analyze using the default architecture module"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != CellData::InstructionType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_ToAsciiString : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "To ASCII string"; }

  virtual std::string GetDescription(void) const
  { return "Make an ASCII string"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != CellData::StringType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class MultiCellAction
{
};

MEDUSA_NAMESPACE_END

#endif //!_MEDUSA_CELL_ACTION_