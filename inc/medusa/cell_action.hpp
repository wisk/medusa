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

class CellAction_Undefine : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Undefine"; }

  virtual std::string GetDescription(void) const
  { return "This option converts the selected item to byte"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != Cell::ValueType || rCell.GetLength() != 1; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_ToWord : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Set to word"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to word type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != Cell::ValueType || rCell.GetLength() != 2; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_ToDword : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Set to dword"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to dword type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != Cell::ValueType || rCell.GetLength() != 4; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_ToQword : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Set to qword"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to qword type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const { return rCell.GetType() != Cell::ValueType || rCell.GetLength() != 8; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_ChangeValueSize : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Change value size"; }

  virtual std::string GetDescription(void) const
  { return "This option allows to change the size of a value (byte, word, dword, qword)"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() == Cell::ValueType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_Analyze : public CellAction
{
public:
  CellAction_Analyze(void) {}
  virtual std::string GetName(void) const
  { return "Analyze"; }

  virtual std::string GetDescription(void) const
  { return "Analyze using the most appropriate architecture"; }

  virtual std::string GetIconName(void) const
  { return "analyze.png"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != Cell::InstructionType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_AnalyzeWith : public CellAction
{
public:
  CellAction_AnalyzeWith(Tag ArchitectureTag, Architecture::NamedMode& rNamedMode)
    : m_ArchTag(ArchitectureTag), m_NamedMode(rNamedMode) {}
  virtual std::string GetName(void) const
  { return std::string("Analyze with ") + std::get<0>(m_NamedMode); }

  virtual std::string GetDescription(void) const
  { return std::string("Analyze using the mode ") + std::get<0>(m_NamedMode); }

  virtual std::string GetIconName(void) const
  { return "analyze.png"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != Cell::InstructionType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);

protected:
  Tag m_ArchTag;
  Architecture::NamedMode m_NamedMode;
};

class CellAction_CreateFunction : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Create function"; }

  virtual std::string GetDescription(void) const
  { return "Create a new function from the current address"; }

  virtual std::string GetIconName(void) const
  { return "function.png"; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() == Cell::InstructionType; } // LATER: check if the function doesn't alreayd exist

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_ToAsciiString : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "To ASCII string"; }

  virtual std::string GetDescription(void) const
  { return "Make an ASCII string"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != Cell::StringType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class CellAction_ToWindowsString : public CellAction
{
public:
  virtual std::string GetName(void) const
  { return "To windows string"; }

  virtual std::string GetDescription(void) const
  { return "Make an windows string"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(Cell const& rCell) const
  { return rCell.GetType() != Cell::StringType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList);
};

class MultiCellAction
{
};

MEDUSA_NAMESPACE_END

#endif //!_MEDUSA_CELL_ACTION_
