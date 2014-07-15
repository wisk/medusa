#include "medusa/cell_action.hpp"
#include "medusa/module.hpp"
#include "medusa/log.hpp"
#include <boost/bind.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

MEDUSA_NAMESPACE_USE

namespace
{

class CellAction_Undefine : public Action
{
public:
  CellAction_Undefine(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_Undefine>(rCore, pView); }

  virtual std::string GetName(void) const
  { return "Undefine"; }

  static char const* GetBindingName(void)
  { return "action.undefine"; }

  virtual std::string GetDescription(void) const
  { return "This option converts the selected item to byte"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    m_rCore.GetDocument().DeleteCell(m_pView->GetCursorAddress());
  }
};

class CellAction_ToWord : public Action
{
public:
  CellAction_ToWord(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_ToWord>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_word"; }

  virtual std::string GetName(void) const
  { return "Set to word"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to word type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    m_rCore.GetDocument().ChangeValueSize(m_pView->GetCursorAddress(), 16, true);
  }
};

class CellAction_ToDword : public Action
{
public:
  CellAction_ToDword(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_ToDword>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_dword"; }

  virtual std::string GetName(void) const
  { return "Set to dword"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to dword type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    m_rCore.GetDocument().ChangeValueSize(m_pView->GetCursorAddress(), 32, true);
  }
};

class CellAction_ToQword : public Action
{
public:
  CellAction_ToQword(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_ToQword>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_qword"; }

  virtual std::string GetName(void) const
  { return "Set to qword"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to qword type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    m_rCore.GetDocument().ChangeValueSize(m_pView->GetCursorAddress(), 64, true);
  }
};

class CellAction_ChangeValueSize : public Action
{
public:
  CellAction_ChangeValueSize(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_ChangeValueSize>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.change_value_size"; }

  virtual std::string GetName(void) const
  { return "Change value size"; }

  virtual std::string GetDescription(void) const
  { return "This option allows to change the size of a value (byte, word, dword, qword)"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    {
      auto const& rAddr = m_pView->GetCursorAddress();
      auto pCell = m_rCore.GetCell(rAddr);
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

      m_rCore.GetDocument().ChangeValueSize(rAddr, NewSize * 8, true);
    }
  }
};

class CellAction_ToCharacter : public Action
{
public:
  CellAction_ToCharacter(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_ToCharacter>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_character"; }

  virtual std::string GetName(void) const
  { return "To character"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to character type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      if (spCell->GetType() == Cell::ValueType)
      {
        spCell->SubType() &= ValueDetail::ModifierMask;
        spCell->SubType() |= ValueDetail::CharacterType;
        m_rCore.GetDocument().SetCell(rAddr, spCell, true);
      }
    }
  }
};

class CellAction_ToReference : public Action
{
public:
  CellAction_ToReference(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_ToReference>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_reference"; }

  virtual std::string GetName(void) const
  { return "To reference"; }

  virtual std::string GetDescription(void) const
  { return "Set the current value to reference type"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      if (spCell->GetType() == Cell::ValueType)
      {
        spCell->SubType() &= ValueDetail::ModifierMask;
        spCell->SubType() |= ValueDetail::ReferenceType;
        m_rCore.GetDocument().SetCell(rAddr, spCell, true);
      }
    }
  }
};

class CellAction_Not: public Action
{
public:
  CellAction_Not(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_Not>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.not_value"; }

  virtual std::string GetName(void) const
  { return "Not"; }

  virtual std::string GetDescription(void) const
  { return "Not the value"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      if (spCell->GetType() == Cell::ValueType)
      {
        spCell->SubType() &= ~ValueDetail::ModifierMask;
        spCell->SubType() ^= ValueDetail::NotType;
        m_rCore.GetDocument().SetCell(rAddr, spCell, true);
      }
    }
  }
};

class CellAction_Negate : public Action
{
public:
  CellAction_Negate(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_Negate>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.negate_value"; }

  virtual std::string GetName(void) const
  { return "Negate"; }

  virtual std::string GetDescription(void) const
  { return "Negate the value"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      if (spCell->GetType() == Cell::ValueType)
      {
        u8 SubType = spCell->GetSubType();
        spCell->SubType() &= ~ValueDetail::ModifierMask;
        spCell->SubType() ^= ValueDetail::NegateType;
        m_rCore.GetDocument().SetCell(rAddr, spCell, true);
      }
    }
  }
};

class CellAction_Binary : public Action
{
public:
  CellAction_Binary(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_Binary>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_binary"; }

  virtual std::string GetName(void) const
  { return "Binary"; }

  virtual std::string GetDescription(void) const
  { return "Display value as binary"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      if (spCell->GetType() == Cell::ValueType)
      {
        u8 SubType = spCell->GetSubType();
        spCell->SubType() &= ~ValueDetail::BaseMask;
        spCell->SubType() |=  ValueDetail::BinaryType;
        m_rCore.GetDocument().SetCell(rAddr, spCell, true);
      }
    }
  }
};

class CellAction_Decimal : public Action
{
public:
  CellAction_Decimal(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_Decimal>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_decimal"; }

  virtual std::string GetName(void) const
  { return "Decimal"; }

  virtual std::string GetDescription(void) const
  { return "Display value as decimal"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      if (spCell->GetType() == Cell::ValueType)
      {
        u8 SubType = spCell->GetSubType();
        spCell->SubType() &= ~ValueDetail::BaseMask;
        spCell->SubType() |=  ValueDetail::DecimalType;
        m_rCore.GetDocument().SetCell(rAddr, spCell, true);
      }
    }
  }
};

class CellAction_Hexadecimal : public Action
{
public:
  CellAction_Hexadecimal(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_Hexadecimal>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_hexadecimal"; }

  virtual std::string GetName(void) const
  { return "Hexadecimal"; }

  virtual std::string GetDescription(void) const
  { return "Display value as hexadecimal"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      if (spCell->GetType() == Cell::ValueType)
      {
        u8 SubType = spCell->GetSubType();
        spCell->SubType() &= ~ValueDetail::BaseMask;
        spCell->SubType() |=  ValueDetail::HexadecimalType;
        m_rCore.GetDocument().SetCell(rAddr, spCell, true);
      }
    }
  }
};

class CellAction_Normal : public Action
{
public:
  CellAction_Normal(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_Normal>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.reset_value"; }

  virtual std::string GetName(void) const
  { return "Normal"; }

  virtual std::string GetDescription(void) const
  { return "Reset value modifier"; }

  virtual std::string GetIconName(void) const
  { return "number.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spCell = m_rCore.GetDocument().GetCell(rAddr);
      if (spCell == nullptr)
        //continue;
          return;
      if (spCell->GetType() == Cell::ValueType)
      {
        u8 SubType = spCell->GetSubType();
        spCell->SubType() &= ~ValueDetail::ModifierMask;
        m_rCore.GetDocument().SetCell(rAddr, spCell, true);
      }
    }
  }
};

class CellAction_Analyze : public Action
{
public:
  CellAction_Analyze(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_Analyze>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.analyze"; }

  virtual std::string GetName(void) const
  { return "Analyze"; }

  virtual std::string GetDescription(void) const
  { return "Analyze using the most appropriate architecture"; }

  virtual std::string GetIconName(void) const
  { return "analyze.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    // TODO: iterate
    auto rAddr = m_pView->GetCursorAddress();
    {
      auto spArch = ModuleManager::Instance().GetArchitecture(m_rCore.GetDocument().GetArchitectureTag(rAddr));
      u8 Mode = m_rCore.GetDocument().GetMode(rAddr);
      if (spArch == nullptr)
      {
        Log::Write("core") << "unable to get architecture for " << rAddr << LogEnd;
        return;
      }
      m_rCore.Analyze(rAddr, spArch, Mode);
    }
  }
};

class CellAction_CreateFunction : public Action
{
public:
  CellAction_CreateFunction(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_CreateFunction>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.create_function"; }

  virtual std::string GetName(void) const
  { return "Create function"; }

  virtual std::string GetDescription(void) const
  { return "Create a new function from the current address"; }

  virtual std::string GetIconName(void) const
  { return "function.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    //m_rCore.CreateFunction(rRangeAddress.second);
  }
};

class CellAction_ToUtf8String : public Action
{
public:
  CellAction_ToUtf8String(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_ToUtf8String>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_utf8_string"; }

  virtual std::string GetName(void) const
  { return "To UTF-8 string"; }

  virtual std::string GetDescription(void) const
  { return "Make an UTF-8 string"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    //Address OldAddr;
    //u64 StrLen = 0;
    //for (auto const& rAddr : rAddrList)
    //{
    //  if (OldAddr + StrLen <= rAddr)
    //  {
    //    m_rCore.MakeAsciiString(rAddr);
    //    auto pStr = m_rCore.GetCell(rAddr);
    //    if (pStr == nullptr) return;
    //    OldAddr = rAddr;
    //    StrLen = pStr->GetLength();
    //  }
    //}
    // TODO: iterate
    m_rCore.MakeAsciiString(m_pView->GetCursorAddress());
  }
};

class CellAction_ToUtf16String : public Action
{
public:
  CellAction_ToUtf16String(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_ToUtf16String>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.to_utf16_name"; }

  virtual std::string GetName(void) const
  { return "To UTF-16 string"; }

  virtual std::string GetDescription(void) const
  { return "Make an UTF-16 string"; }

  virtual std::string GetIconName(void) const
  { return ""; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    //Address OldAddr;
    //u64 StrLen = 0;
    //for (auto const& rAddr : rAddrList)
    //{
    //  if (OldAddr + StrLen <= rAddr)
    //  {
    //    m_rCore.MakeWindowsString(rAddr);
    //    auto pStr = m_rCore.GetCell(rAddr);
    //    if (pStr == nullptr) return;
    //    OldAddr = rAddr;
    //    StrLen = pStr->GetLength();
    //  }
    //}
    // TODO: iterate
    m_rCore.MakeWindowsString(m_pView->GetCursorAddress());
  }
};


class CellAction_GoToPreviousAddress : public Action
{
public:
  CellAction_GoToPreviousAddress(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_GoToPreviousAddress>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.go_to_previous_address"; }

  virtual std::string GetName(void) const
  { return "Go to previous address"; }

  virtual std::string GetDescription(void) const
  { return "Get back to the previous address"; }

  virtual std::string GetIconName(void) const
  { return "previous_address.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    auto& rDoc = m_rCore.GetDocument();
    Address PrevAddr;
    if (!rDoc.GetPreviousAddressInHistory(PrevAddr))
    {
      Log::Write("core") << "unable to get previous address" << LogEnd;
      return;
    }

    Log::Write("core") << "previous address: " << PrevAddr << LogEnd;
    m_pView->GoTo(PrevAddr, false);
  }
};

class CellAction_GoToNextAddress : public Action
{
public:
  CellAction_GoToNextAddress(Medusa& rCore, FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return std::make_shared<CellAction_GoToNextAddress>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.go_to_next_address"; }

  virtual std::string GetName(void) const
  { return "Go to next address"; }

  virtual std::string GetDescription(void) const
  { return "Go to the next address"; }

  virtual std::string GetIconName(void) const
  { return "next_address.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    auto& rDoc = m_rCore.GetDocument();
    Address NextAddr;
    if (!rDoc.GetNextAddressInHistory(NextAddr))
    {
      Log::Write("core") << "unable to get next address" << LogEnd;
      return;
    }

    Log::Write("core") << "next address: " << NextAddr << LogEnd;
    m_pView->GoTo(NextAddr, false);
  }
};

}

Action::MapType Action::GetMap(void)
{
  static MapType s_Actions;
  if (s_Actions.empty())
  {
    s_Actions[CellAction_Undefine::GetBindingName()]            = &CellAction_Undefine::Create;
    s_Actions[CellAction_ChangeValueSize::GetBindingName()]     = &CellAction_ChangeValueSize::Create;
    s_Actions[CellAction_ToWord::GetBindingName()]              = &CellAction_ToWord::Create;
    s_Actions[CellAction_ToDword::GetBindingName()]             = &CellAction_ToDword::Create;
    s_Actions[CellAction_ToQword::GetBindingName()]             = &CellAction_ToQword::Create;
    s_Actions[CellAction_ToReference::GetBindingName()]         = &CellAction_ToReference::Create;
    s_Actions[CellAction_ToCharacter::GetBindingName()]         = &CellAction_ToCharacter::Create;
    s_Actions[CellAction_Normal::GetBindingName()]              = &CellAction_Normal::Create;
    s_Actions[CellAction_Not::GetBindingName()]                 = &CellAction_Not::Create;
    s_Actions[CellAction_Negate::GetBindingName()]              = &CellAction_Negate::Create;
    s_Actions[CellAction_Binary::GetBindingName()]              = &CellAction_Binary::Create;
    s_Actions[CellAction_Decimal::GetBindingName()]             = &CellAction_Decimal::Create;
    s_Actions[CellAction_Hexadecimal::GetBindingName()]         = &CellAction_Hexadecimal::Create;
    s_Actions[CellAction_Analyze::GetBindingName()]             = &CellAction_Analyze::Create;
    s_Actions[CellAction_CreateFunction::GetBindingName()]      = &CellAction_CreateFunction::Create;
    s_Actions[CellAction_ToUtf8String::GetBindingName()]        = &CellAction_ToUtf8String::Create;
    s_Actions[CellAction_ToUtf16String::GetBindingName()]       = &CellAction_ToUtf16String::Create;
    s_Actions[CellAction_GoToPreviousAddress::GetBindingName()] = &CellAction_GoToPreviousAddress::Create;
    s_Actions[CellAction_GoToNextAddress::GetBindingName()]     = &CellAction_GoToNextAddress::Create;
  }
  return s_Actions;
}

class CellAction_AnalyzeWith : public Action
{
public:
  CellAction_AnalyzeWith(Medusa& rCore, FullDisassemblyView* pView, Tag ArchitectureTag, Architecture::NamedMode& rNamedMode)
    : Action(rCore, pView), m_ArchTag(ArchitectureTag), m_NamedMode(rNamedMode) {}

  static SPtr Create(Medusa& rCore, FullDisassemblyView* pView)
  { return nullptr; }

  static char const* GetBindingName(void)
  { return nullptr; }

  virtual std::string GetName(void) const
  { return std::string("Analyze with ") + std::get<0>(m_NamedMode); }

  virtual std::string GetDescription(void) const
  { return std::string("Analyze using the mode ") + std::get<0>(m_NamedMode); }

  virtual std::string GetIconName(void) const
  { return "analyze.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void)
  {
    auto spArch = ModuleManager::Instance().GetArchitecture(m_ArchTag);
    if (spArch == nullptr)
    {
      Log::Write("core") << "unable to retrieve arch using tag: " << m_ArchTag << LogEnd;
      return;
    }

    // TODO: iterate...
    u8 Mode = std::get<1>(m_NamedMode);
    m_rCore.Analyze(m_pView->GetCursorAddress(), spArch, Mode);
  }

protected:
  Tag m_ArchTag;
  Architecture::NamedMode m_NamedMode;
};

Action::SPtrList Action::GetSpecificActions(Medusa& rCore, FullDisassemblyView* pView, Address const& rAddress)
{
  SPtrList Actions;

  auto& rModMgr = ModuleManager::Instance();

  auto const ArchTags = rCore.GetDocument().GetArchitectureTags();
  for (auto ArchTag : ArchTags)
  {
    auto spArch = rModMgr.GetArchitecture(ArchTag);
    if (spArch == nullptr)
      continue;

    auto ArchModes = spArch->GetModes();

    for (auto& rArchMode : ArchModes)
    {
      Actions.push_back(std::make_shared<CellAction_AnalyzeWith>(rCore, pView, ArchTag, rArchMode));
    }
  }

  return Actions;
}