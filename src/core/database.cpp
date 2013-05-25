#include "medusa/database.hpp"
#include "medusa/medusa.hpp"
#include "medusa/value.hpp"
#include "medusa/log.hpp"

#include <boost/foreach.hpp>

MEDUSA_NAMESPACE_BEGIN

Database::Database(FileBinaryStream const& rBinaryStream)
  : m_rBinaryStream(rBinaryStream)
{
}

Database::~Database(void)
{
  StopsEventHandling();
  RemoveAll();
}

MemoryArea* Database::GetMemoryArea(Address const& rAddr)
{
  boost::lock_guard<MutexType> Lock(m_MemoryAreaMutex);
  for (TMemoryAreas::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsPresent(rAddr))
      return *It;

  return NULL;
}

MemoryArea const* Database::GetMemoryArea(Address const& rAddr) const
{
  boost::lock_guard<MutexType> Lock(m_MemoryAreaMutex);
  for (TMemoryAreas::const_iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsPresent(rAddr))
      return *It;

  return NULL;
}

Label Database::GetLabelFromAddress(Address const& rAddr) const
{
  TLabelMap::left_const_iterator Iter = m_LabelMap.left.find(rAddr);

  if (Iter == m_LabelMap.left.end())
    return Label("", Label::LabelUnknown);

  return Iter->second;
}

void Database::SetLabelToAddress(Address const& rAddr, Label const& rLabel)
{
  TLabelMap::left_iterator Iter = m_LabelMap.left.find(rAddr);
  m_LabelMap.left.replace_data(Iter, rLabel);
  m_EventQueue.Push(EventHandler::LabelUpdated(rLabel, EventHandler::LabelUpdated::Add));
}

Address Database::GetAddressFromLabelName(std::string const& rLabelName) const
{
  TLabelMap::right_const_iterator Iter = m_LabelMap.right.find(Label(rLabelName, Label::LabelUnknown));

  if (Iter == m_LabelMap.right.end())
    return Address();

  return Iter->second;
}

void Database::AddLabel(Address const& rAddr, Label const& rLabel)
{
  m_LabelMap.insert(TLabelMap::value_type(rAddr, rLabel));
  m_EventQueue.Push(EventHandler::LabelUpdated(rLabel, EventHandler::LabelUpdated::Add));
}

bool Database::ChangeValueSize(Address const& rValueAddr, u8 NewValueSize, bool Force)
{
  Cell* pOldCell = RetrieveCell(rValueAddr);

  if (pOldCell == nullptr) return false;

  if (pOldCell->GetType() == CellData::InstructionType && Force == false)
    return false;

  NewValueSize /= 8;

  size_t OldCellLength = pOldCell->GetLength();
  if (pOldCell->GetType() == CellData::ValueType && OldCellLength == NewValueSize)
    return true;

  u32 ValueType = (static_cast<Value*>(pOldCell)->GetValueType() & VT_MASK);

  switch (NewValueSize)
  {
  case 1: ValueType |= VS_8BIT;  break;
  case 2: ValueType |= VS_16BIT; break;
  case 4: ValueType |= VS_32BIT; break;
  case 8: ValueType |= VS_64BIT; break;
  default: return false;
  }

  Cell* pNewCell = new Value(ValueType);

  if (NewValueSize > OldCellLength)
    return InsertCell(rValueAddr, pNewCell, Force);

  if (InsertCell(rValueAddr, pNewCell, Force) == false)
    return false;

  for (u32 i = NewValueSize; i < OldCellLength; ++i)
    if (InsertCell(rValueAddr + i, new Value(ValueType | VS_8BIT), Force) == false)
      return false;

  return true;
}

bool Database::MakeAsciiString(Address const& rAddr)
{
  try
  {
    s8 CurChar;
    TOffset StrOff;
    std::string StrData = "";
    auto pMemArea       = GetMemoryArea(rAddr);
    auto rCurBinStrm    = pMemArea->GetBinaryStream();

    if (pMemArea->Convert(rAddr.GetOffset(), StrOff) == false)
      return false;

    for (;;)
    {
      rCurBinStrm.Read(StrOff, CurChar);
      if (CurChar == '\0') break;

      StrData += CurChar;
      ++StrOff;
    }

    if (StrData.length() == 0) return false;

    auto pStr = new String(String::AsciiType, StrData);
    InsertCell(rAddr, pStr, true);
  }
  catch (Exception const&)
  {
    return false;
  }

  return true;
}

bool Database::MakeWindowsString(Address const& rAddr)
{
  try
  {
    TOffset StrStartOff, StrOff;
    std::string StrData = "";
    auto pMemArea       = GetMemoryArea(rAddr);
    auto rCurBinStrm    = pMemArea->GetBinaryStream();
    WinString WinStr;
    WinString::CharType CurChar;

    if (pMemArea->Convert(rAddr.GetOffset(), StrOff) == false)
      return false;

    StrStartOff = StrOff;

    bool EndReached = false;
    do
    {
      rCurBinStrm.Read(StrOff, CurChar);
      if (WinStr.IsFinalCharacter(CurChar)) EndReached = true;

      if (EndReached == false)
        StrData += WinStr.ConvertToUf8(CurChar);
      StrOff += sizeof(CurChar);
    } while (EndReached == false);

    if (StrData.length() == 0) return false;

    auto pStr = new String(String::Utf16Type, StrData, static_cast<u16>(StrOff - StrStartOff));
    InsertCell(rAddr, pStr, true);
  }
  catch (Exception const&)
  {
    return false;
  }

  return true;
}

Cell* Database::RetrieveCell(Address const& rAddr)
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == NULL)
    return NULL;

  return pMemArea->RetrieveCell(rAddr.GetOffset());
}

Cell const* Database::RetrieveCell(Address const& rAddr) const
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea const* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == NULL)
    return NULL;

  return pMemArea->RetrieveCell(rAddr.GetOffset());
}

bool Database::InsertCell(Address const& rAddr, Cell* pCell, bool Force, bool Safe)
{
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == NULL)
    return false;

  Address::List ErasedAddresses;
  if (!pMemArea->InsertCell(rAddr.GetOffset(), pCell, ErasedAddresses, Force, Safe))
    return false;

  for (auto itAddr = std::begin(ErasedAddresses); itAddr != std::end(ErasedAddresses); ++itAddr)
    if (RetrieveCell(*itAddr) == nullptr)
    {
      Address DstAddr;
      if (GetXRefs().To(*itAddr, DstAddr))
        GetXRefs().RemoveRef(*itAddr);

      Address::List ListAddr;
      GetXRefs().From(*itAddr, ListAddr);
      if (ListAddr.size() != 0)
      {
        auto Label = GetLabelFromAddress(*itAddr);
        if (Label.GetType() != Label::LabelUnknown)
          m_EventQueue.Push(EventHandler::LabelUpdated(Label, EventHandler::LabelUpdated::Remove));
      }
    }

  m_EventQueue.Push(EventHandler::DatabaseUpdated());

  return true;
}

void Database::UpdateCell(Address const& rAddr, Cell* pCell)
{
  m_EventQueue.Push(EventHandler::DatabaseUpdated());
}

MultiCell* Database::RetrieveMultiCell(Address const& rAddr)
{
  MultiCell::Map::iterator itMultiCell = m_MultiCells.find(rAddr);
  if (itMultiCell == m_MultiCells.end())
    return NULL;

  return itMultiCell->second;
}

MultiCell const* Database::RetrieveMultiCell(Address const& rAddr) const
{
  MultiCell::Map::const_iterator itMultiCell = m_MultiCells.find(rAddr);
  if (itMultiCell == m_MultiCells.end())
    return NULL;

  return itMultiCell->second;
}

bool Database::InsertMultiCell(Address const& rAddr, MultiCell* pMultiCell, bool Force)
{
  if (Force)
  {
    m_MultiCells[rAddr] = pMultiCell;
    return true;
  }

  MultiCell::Map::iterator itMultiCell = m_MultiCells.find(rAddr);
  if (itMultiCell != m_MultiCells.end())
    return false;

  m_MultiCells[rAddr] = pMultiCell;
  m_EventQueue.Push(EventHandler::DatabaseUpdated());
  return true;
}

bool Database::Translate(Address const& rAddr, TOffset& rRawOffset) const
{
  MemoryArea const* pMemoryArea = GetMemoryArea(rAddr);
  if (pMemoryArea == NULL)
    return false;

  return pMemoryArea->Translate(rAddr.GetOffset(), rRawOffset);
}

bool Database::Convert(Address const& rAddr, TOffset& rMemAreaOffset) const
{
  MemoryArea const* pMemoryArea = GetMemoryArea(rAddr);
  if (pMemoryArea == NULL)
    return false;

  return pMemoryArea->Convert(rAddr.GetOffset(), rMemAreaOffset);
}

void Database::StartsEventHandling(EventHandler* pEvtHdl)
{
  m_Thread = boost::thread(&Database::ProcessEventQueue, this, pEvtHdl);
}

void Database::StopsEventHandling(void)
{
  m_EventQueue.Quit();
  m_Thread.join();
}

void Database::ProcessEventQueue(EventHandler* pEvtHdl)
{
  m_EventQueue.ProcessQueue(*pEvtHdl);
}

void Database::RemoveAll(void)
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  for (TMemoryAreas::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    delete *It;
  m_MemoryAreas.erase(m_MemoryAreas.begin(), m_MemoryAreas.end());

  m_MultiCells.erase(m_MultiCells.begin(), m_MultiCells.end());
  m_LabelMap.erase(m_LabelMap.begin(), m_LabelMap.end());
  m_XRefs.EraseAll();
}

void Database::AddMemoryArea(MemoryArea* pMemoryArea)
{
  m_MemoryAreas.insert(pMemoryArea);

  for (auto itCell = pMemoryArea->Begin(); itCell != pMemoryArea->End(); ++itCell)
  {
    if (itCell->second == nullptr) continue;
    medusa::Address CurAddr(itCell->first);
    pMemoryArea->FormatAddress(CurAddr);
  }
}

bool Database::IsPresent(Address const& Addr) const
{
  for (TMemoryAreas::const_iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsPresent(Addr))
      return true;

  return false;
}

bool Database::Read(Address const& rAddress, void* pBuffer, u32 Size) const
{
  MemoryArea const* pMemoryArea = GetMemoryArea(rAddress);
  if (pMemoryArea == NULL)
    return false;
  return pMemoryArea->Read(rAddress.GetOffset(), pBuffer, Size);
}

bool Database::Write(Address const& rAddress, void const* pBuffer, u32 Size)
{
  MemoryArea* pMemoryArea = GetMemoryArea(rAddress);
  if (pMemoryArea == NULL)
    return false;
  return pMemoryArea->Write(rAddress.GetOffset(), pBuffer, Size);
}

u32 Database::GetNumberOfAddress(void) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  u32 Res = 0;
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
    Res += static_cast<u32>((*itMemArea)->GetSize());
  return Res;
}

bool Database::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  if (Offset < 0)
    return MoveAddressBackward(rAddress, rMovedAddress, Offset);
  if (Offset > 0)
    return MoveAddressForward(rAddress, rMovedAddress, Offset);
  rMovedAddress = rAddress;
  return true;
}

bool Database::MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  // FIXME: Handle Offset
  if (rAddress <= (*m_MemoryAreas.begin())->GetVirtualBase())
  {
    rMovedAddress = rAddress;
    return true;
  }

  auto itMemArea = std::begin(m_MemoryAreas);
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsPresent(rAddress))
      break;
  }
  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  u64 CurMemAreaOff = (rAddress.GetOffset() - (*itMemArea)->GetVirtualBase().GetOffset());
  if (static_cast<u64>(-Offset) <= CurMemAreaOff)
    return (*itMemArea)->MoveAddressBackward(rAddress, rMovedAddress, Offset);
  Offset += CurMemAreaOff;

  if (itMemArea == std::begin(m_MemoryAreas))
    return false;
  --itMemArea;

  bool Failed = false;
  Address CurAddr = ((*itMemArea)->GetVirtualBase() + ((*itMemArea)->GetSize() - 1));
  while (itMemArea != std::begin(m_MemoryAreas))
  {
    u64 MemAreaSize = (*itMemArea)->GetSize();
    if (static_cast<u64>(-Offset) < MemAreaSize)
      break;
    Offset += MemAreaSize;
    CurAddr = ((*itMemArea)->GetVirtualBase() + ((*itMemArea)->GetSize() - 1));

    if (itMemArea == std::begin(m_MemoryAreas))
      return false;

    --itMemArea;
  }

  return (*itMemArea)->MoveAddressBackward(CurAddr, rMovedAddress, Offset);
}

bool Database::MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  auto itMemArea = std::begin(m_MemoryAreas);
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsPresent(rAddress))
      break;
  }
  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  u64 CurMemAreaOff = (rAddress.GetOffset() - (*itMemArea)->GetVirtualBase().GetOffset());
  if (CurMemAreaOff + Offset < (*itMemArea)->GetSize())
    if ((*itMemArea)->MoveAddressForward(rAddress, rMovedAddress, Offset) == true)
      return true;

  s64 DiffOff = ((*itMemArea)->GetSize() - CurMemAreaOff);
  if (DiffOff >= Offset)
    Offset = 0;
  else
    Offset -= DiffOff;
  ++itMemArea;

  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  Address CurAddr = (*itMemArea)->GetVirtualBase();
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    u64 MemAreaSize = (*itMemArea)->GetSize();
    if (static_cast<u64>(Offset) < MemAreaSize)
      if ((*itMemArea)->MoveAddressForward(CurAddr, rMovedAddress, Offset) == true)
        return true;
    Offset -= MemAreaSize;
    CurAddr = (*itMemArea)->GetVirtualBase();
  }

  return false;
}

bool Database::GetNextAddress(Address const& rAddress, Address& rNextAddress) const
{
  return MoveAddressForward(rAddress, rNextAddress, 1);

  auto itMemArea = std::begin(m_MemoryAreas);
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsPresent(rAddress))
      break;
  }

  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  if (((*itMemArea)->End() - 1)->first == rAddress.GetOffset())
  {
    ++itMemArea;
    if (itMemArea == std::end(m_MemoryAreas))
      return false;
    rNextAddress = (*itMemArea)->GetVirtualBase();
    return true;
  }

  return (*itMemArea)->GetNextAddress(rAddress, rNextAddress);
}

bool Database::GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const
{
  auto itMemArea = std::begin(m_MemoryAreas);

  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if (rAddress < (*itMemArea)->GetVirtualBase())
    {
      rNearestAddress = (*itMemArea)->GetVirtualBase();
      return true;
    }

    if ((*itMemArea)->IsPresent(rAddress))
      return (*itMemArea)->GetNearestAddress(rAddress, rNearestAddress);
  }

  return false;
}

bool Database::ConvertPositionToAddress(u64 Position, Address& rAddress) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  Address FirstAddress = (*m_MemoryAreas.begin())->GetVirtualBase();
  return MoveAddressForward(FirstAddress, rAddress, Position);
}

bool Database::ConvertAddressToPosition(Address const& rAddress, u64& rPosition) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  rPosition = 0;
  auto itMemArea = std::begin(m_MemoryAreas);
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsPresent(rAddress))
      break;
    rPosition += (*itMemArea)->GetSize();
  }

  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  auto itCell = (*itMemArea)->Begin();
  auto itEndCell = (*itMemArea)->End();
  auto Offset = rAddress.GetOffset();
  for (; itCell != itEndCell; ++itCell)
  {
    if (itCell->second == nullptr)
      continue;
    if (itCell->first == Offset)
      break;
    ++rPosition;
  }
  
  return itCell != itEndCell;
}

void Database::FindFunctionAddressFromAddress(Address::List& rFunctionAddress, Address const& rAddress) const
{
  for (auto itMc = std::begin(m_MultiCells); itMc != std::end(m_MultiCells); ++itMc)
  {
    auto pFunction = dynamic_cast<Function const*>(itMc->second);
    if (pFunction == nullptr)
      continue;

    if (pFunction->Contains(rAddress) == false)
      continue;

    rFunctionAddress.push_back(itMc->first);
  }
}

MEDUSA_NAMESPACE_END
