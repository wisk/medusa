#include <medusa/cell_action.hpp>
#include <boost/bind.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

MEDUSA_NAMESPACE_USE

void CellAction::GetCellActionBuilders(CellAction::PtrList& rActList)
{
  rActList.push_back(new CellAction_Undefine);
  rActList.push_back(new CellAction_ChangeValueSize);
  rActList.push_back(new CellAction_ToWord);
  rActList.push_back(new CellAction_ToDword);
  rActList.push_back(new CellAction_ToQword);
  rActList.push_back(new CellAction_Disassemble);
  rActList.push_back(new CellAction_Analyze);
  rActList.push_back(new CellAction_CreateFunction);
  rActList.push_back(new CellAction_ToAsciiString);
  rActList.push_back(new CellAction_ToWindowsString);
}

void CellAction::GetCellActionBuilders(Cell const& rCell, CellAction::PtrList& rActList)
{
  GetCellActionBuilders(rActList);
  boost::remove_erase_if(rActList, !boost::bind(&CellAction::IsCompatible, _1, rCell));
}

void CellAction_Undefine::Do(Medusa& rCore, Address::List const& rAddrList)
{
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&rCore, &rAddrList](Address const& rAddr)
  {
    rCore.GetDocument().ChangeValueSize(rAddr, 8, true);
  });
}

void CellAction_ToWord::Do(Medusa& rCore, Address::List const& rAddrList)
{
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&rCore, &rAddrList](Address const& rAddr)
  {
    rCore.GetDocument().ChangeValueSize(rAddr, 16, true);
  });
}

void CellAction_ToDword::Do(Medusa& rCore, Address::List const& rAddrList)
{
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&rCore, &rAddrList](Address const& rAddr)
  {
    rCore.GetDocument().ChangeValueSize(rAddr, 32, true);
  });
}

void CellAction_ToQword::Do(Medusa& rCore, Address::List const& rAddrList)
{
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&rCore, &rAddrList](Address const& rAddr)
  {
    rCore.GetDocument().ChangeValueSize(rAddr, 64, true);
  });
}


void CellAction_ChangeValueSize::Do(Medusa& rCore, Address::List const& rAddrList)
{
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&rCore, &rAddrList](Address const& rAddr)
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
  });
}

void CellAction_Disassemble::Do(Medusa& rCore, Address::List const& rAddrList)
{
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&rCore, &rAddrList](Address const& rAddr)
  {
    rCore.DisassembleAsync(rAddr);
  });
}

void CellAction_Analyze::Do(Medusa& rCore, Address::List const& rAddrList)
{
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&rCore, &rAddrList](Address const& rAddr)
  {
    rCore.AnalyzeAsync(rAddr);
  });
}

void CellAction_CreateFunction::Do(Medusa& rCore, Address::List const& rAddrList)
{
  if (rAddrList.empty())
    return;

  rCore.CreateFunction(*rAddrList.begin());
}

void CellAction_ToAsciiString::Do(Medusa& rCore, Address::List const& rAddrList)
{
  Address OldAddr;
  u64 StrLen = 0;
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&](Address const& rAddr)
  {
    if (OldAddr + StrLen <= rAddr)
    {
      rCore.MakeAsciiString(rAddr);
      auto pStr = rCore.GetCell(rAddr);
      if (pStr == nullptr) return;
      OldAddr = rAddr;
      StrLen = pStr->GetLength();
    }
  });
}

void CellAction_ToWindowsString::Do(Medusa& rCore, Address::List const& rAddrList)
{
  Address OldAddr;
  u64 StrLen = 0;
  std::for_each(std::begin(rAddrList), std::end(rAddrList), [&](Address const& rAddr)
  {
    if (OldAddr + StrLen <= rAddr)
    {
      rCore.MakeWindowsString(rAddr);
      auto pStr = rCore.GetCell(rAddr);
      if (pStr == nullptr) return;
      OldAddr = rAddr;
      StrLen = pStr->GetLength();
    }
  });
}
