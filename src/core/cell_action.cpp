#include "medusa/cell_action.hpp"
#include "medusa/module.hpp"
#include "medusa/log.hpp"
#include <boost/bind.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

MEDUSA_NAMESPACE_USE

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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      rCore.GetDocument().ChangeValueSize(rAddr, 8, true);
    }
  }
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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      rCore.GetDocument().ChangeValueSize(rAddr, 16, true);
    }
  }
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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      rCore.GetDocument().ChangeValueSize(rAddr, 32, true);
    }
  }
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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      rCore.GetDocument().ChangeValueSize(rAddr, 64, true);
    }
  }
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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      auto pCell = rCore.GetCell(rAddr);
      if (pCell == nullptr) return;

      u8 NewSize = 0;
      switch (pCell->GetLength())
      {
      case 1: NewSize = 2; break;
      case 2: NewSize = 4; break;
      case 4: NewSize = 8; break;
      case 8: NewSize = 1; break;
      default: return;
      }

      rCore.GetDocument().ChangeValueSize(rAddr, NewSize * 8, true);
    }
  }
};

class CellAction_ToCharacter : public CellAction
{
  virtual std::string GetName(void) const
  { return "To character"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to character type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const { return rCell.GetType() == Cell::ValueType && rCell.GetLength() <= 2; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      auto spCell = rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        continue;
      spCell->SubType() &= Value::ModifierMask;
      spCell->SubType() |= Value::CharacterType;
      rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_ToReference : public CellAction
{
  virtual std::string GetName(void) const
  { return "To reference"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to reference type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const { return rCell.GetType() == Cell::ValueType && rCell.GetLength() >= 2; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      auto spCell = rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        continue;
      spCell->SubType() &= Value::ModifierMask;
      spCell->SubType() |= Value::ReferenceType;
      rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_Not: public CellAction
{
  virtual std::string GetName(void) const
  { return "Not"; }

  virtual std::string GetDescription(void) const
  { return "Not the value"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const { return rCell.GetType() == Cell::ValueType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      auto spCell = rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        continue;
      spCell->SubType() &= Value::BaseMask;
      spCell->SubType() |= Value::NotType;
      rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_Negate : public CellAction
{
  virtual std::string GetName(void) const
  { return "Negate"; }

  virtual std::string GetDescription(void) const
  { return "Negate the value"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const { return rCell.GetType() == Cell::ValueType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      auto spCell = rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        continue;
      u8 SubType = spCell->GetSubType();
      spCell->SubType() &= Value::ModifierMask;
      spCell->SubType() |= Value::NegateType;
      rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
};

class CellAction_Normal : public CellAction
{
  virtual std::string GetName(void) const
  { return "Normal"; }

  virtual std::string GetDescription(void) const
  { return "Reset value modifier"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(Cell const& rCell) const { return rCell.GetType() == Cell::ValueType; }

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      auto spCell = rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        continue;
      u8 SubType = spCell->GetSubType();
      spCell->SubType() &= Value::BaseMask;
      rCore.GetDocument().SetCell(rAddr, spCell, true);
    }
  }
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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    for (auto const& rAddr : rAddrList)
    {
      auto spArch = ModuleManager::Instance().GetArchitecture(rCore.GetDocument().GetArchitectureTag(rAddr));
      u8 Mode = rCore.GetDocument().GetMode(rAddr);
      if (spArch == nullptr)
      {
        Log::Write("core") << "unable to get architecture for " << rAddr << LogEnd;
        return;
      }
      rCore.Analyze(rAddr, spArch, Mode);
    }
  }
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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    auto spArch = ModuleManager::Instance().GetArchitecture(m_ArchTag);
    if (spArch == nullptr)
      return; // TODO: Log error
    u8 Mode = std::get<1>(m_NamedMode);
    for (auto const& rAddr : rAddrList)
    {
      rCore.Analyze(rAddr, spArch, Mode);
    }
  }

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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    if (rAddrList.empty())
      return;

    rCore.CreateFunction(*rAddrList.begin());
  }
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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    Address OldAddr;
    u64 StrLen = 0;
    for (auto const& rAddr : rAddrList)
    {
      if (OldAddr + StrLen <= rAddr)
      {
        rCore.MakeAsciiString(rAddr);
        auto pStr = rCore.GetCell(rAddr);
        if (pStr == nullptr) return;
        OldAddr = rAddr;
        StrLen = pStr->GetLength();
      }
    }
  }
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

  virtual void Do(Medusa& rCore, Address::List const& rAddrList)
  {
    Address OldAddr;
    u64 StrLen = 0;
    for (auto const& rAddr : rAddrList)
    {
      if (OldAddr + StrLen <= rAddr)
      {
        rCore.MakeWindowsString(rAddr);
        auto pStr = rCore.GetCell(rAddr);
        if (pStr == nullptr) return;
        OldAddr = rAddr;
        StrLen = pStr->GetLength();
      }
    }
  }
};

void CellAction::GetCellActionBuilders(Medusa const& rCore, Address const& rAddress, CellAction::PtrList& rActList)
{
  rActList.push_back(new CellAction_Undefine);
  rActList.push_back(new CellAction_ChangeValueSize);
  rActList.push_back(new CellAction_ToWord);
  rActList.push_back(new CellAction_ToDword);
  rActList.push_back(new CellAction_ToQword);
  rActList.push_back(new CellAction_ToReference);
  rActList.push_back(new CellAction_ToCharacter);
  rActList.push_back(new CellAction_Normal);
  rActList.push_back(new CellAction_Not);
  rActList.push_back(new CellAction_Negate);

  rActList.push_back(new CellAction_Analyze);

  std::list<Tag> const Tags = rCore.GetDocument().GetArchitectureTags();
  for (auto itTag = std::begin(Tags), itEnd = std::end(Tags); itTag != itEnd; ++itTag)
  {
    auto spArch = ModuleManager::Instance().GetArchitecture(*itTag);
    if (spArch == nullptr)
      continue;
    Architecture::NamedModeVector AvailableModes = spArch->GetModes();
    for (auto itMode = std::begin(AvailableModes), itEnd = std::end(AvailableModes); itMode != itEnd; ++itMode)
      rActList.push_back(new CellAction_AnalyzeWith(spArch->GetTag(), *itMode));
  }

  rActList.push_back(new CellAction_CreateFunction);
  rActList.push_back(new CellAction_ToAsciiString);
  rActList.push_back(new CellAction_ToWindowsString);

  //boost::remove_erase_if(rActList, !boost::bind(&CellAction::IsCompatible, _1, *spCell));
}