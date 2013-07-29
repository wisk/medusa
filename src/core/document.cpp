#include "medusa/document.hpp"
#include "medusa/medusa.hpp"
#include "medusa/value.hpp"
#include "medusa/log.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

MEDUSA_NAMESPACE_BEGIN

Document::Document(FileBinaryStream const& rBinaryStream)
  : m_rBinaryStream(rBinaryStream)
{
}

Document::~Document(void)
{
  m_QuitSignal();
  RemoveAll();
}

void Document::RemoveAll(void)
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  for (MemoryAreaSetType::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    delete *It;
  m_MemoryAreas.erase(m_MemoryAreas.begin(), m_MemoryAreas.end());

  m_MultiCells.erase(m_MultiCells.begin(), m_MultiCells.end());
  m_LabelMap.erase(m_LabelMap.begin(), m_LabelMap.end());
  m_XRefs.EraseAll();
}

void Document::Connect(u32 Type, Document::Subscriber* pSubscriber)
{
  if (Type & Subscriber::Quit)
    pSubscriber->m_QuitConnection = m_QuitSignal.connect(boost::bind(&Subscriber::OnQuit, pSubscriber));

  if (Type & Subscriber::DocumentUpdated)
    pSubscriber->m_DocumentUpdatedConnection = m_DocumentUpdatedSignal.connect(boost::bind(&Subscriber::OnDocumentUpdated, pSubscriber));

  if (Type & Subscriber::LabelUpdated)
    pSubscriber->m_LabelUpdatedConnection = m_LabelUpdatedSignal.connect(boost::bind(&Subscriber::OnLabelUpdated, pSubscriber, _1, _2));
}

MemoryArea* Document::GetMemoryArea(Address const& rAddr)
{
  boost::lock_guard<MutexType> Lock(m_MemoryAreaMutex);
  for (MemoryAreaSetType::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsPresent(rAddr))
      return *It;

  return nullptr;
}

MemoryArea const* Document::GetMemoryArea(Address const& rAddr) const
{
  boost::lock_guard<MutexType> Lock(m_MemoryAreaMutex);
  for (MemoryAreaSetType::const_iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsPresent(rAddr))
      return *It;

  return nullptr;
}

Label Document::GetLabelFromAddress(Address const& rAddr) const
{
  LabelBimapType::left_const_iterator Iter = m_LabelMap.left.find(rAddr);

  if (Iter == m_LabelMap.left.end())
    return Label("", Label::LabelUnknown);

  return Iter->second;
}

void Document::SetLabelToAddress(Address const& rAddr, Label const& rLabel)
{
  LabelBimapType::left_iterator Iter = m_LabelMap.left.find(rAddr);
  m_LabelMap.left.replace_data(Iter, rLabel);
  m_LabelUpdatedSignal(rLabel, false);
}

Address Document::GetAddressFromLabelName(std::string const& rLabelName) const
{
  LabelBimapType::right_const_iterator Iter = m_LabelMap.right.find(Label(rLabelName, Label::LabelUnknown));

  if (Iter == m_LabelMap.right.end())
    return Address();

  return Iter->second;
}

void Document::AddLabel(Address const& rAddr, Label const& rLabel)
{
  auto const& rOldLabel = GetLabelFromAddress(rAddr);
  if (rOldLabel.GetType() != Label::LabelUnknown)
    return;
  m_LabelMap.insert(LabelBimapType::value_type(rAddr, rLabel));
  m_LabelUpdatedSignal(rLabel, false);
}

bool Document::ChangeValueSize(Address const& rValueAddr, u8 NewValueSize, bool Force)
{
  Cell* pOldCell = RetrieveCell(rValueAddr);

  if (pOldCell == nullptr)
    return false;

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

bool Document::MakeAsciiString(Address const& rAddr)
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

bool Document::MakeWindowsString(Address const& rAddr)
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

Cell* Document::RetrieveCell(Address const& rAddr)
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
    return nullptr;

  m_LastAddressAccessed = rAddr;
  return pMemArea->RetrieveCell(rAddr.GetOffset());
}

Cell const* Document::RetrieveCell(Address const& rAddr) const
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea const* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
    return nullptr;

  return pMemArea->RetrieveCell(rAddr.GetOffset());
}

bool Document::InsertCell(Address const& rAddr, Cell* pCell, bool Force, bool Safe)
{
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
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
        {
          m_LabelUpdatedSignal(Label, true);
        }
      }
    }

  m_DocumentUpdatedSignal();

  m_LastAddressAccessed = rAddr;
  return true;
}

void Document::UpdateCell(Address const& rAddr, Cell* pCell)
{
  m_DocumentUpdatedSignal();
}

MultiCell* Document::RetrieveMultiCell(Address const& rAddr)
{
  MultiCell::Map::iterator itMultiCell = m_MultiCells.find(rAddr);
  if (itMultiCell == m_MultiCells.end())
    return nullptr;

  return itMultiCell->second;
}

MultiCell const* Document::RetrieveMultiCell(Address const& rAddr) const
{
  MultiCell::Map::const_iterator itMultiCell = m_MultiCells.find(rAddr);
  if (itMultiCell == m_MultiCells.end())
    return nullptr;

  return itMultiCell->second;
}

bool Document::InsertMultiCell(Address const& rAddr, MultiCell* pMultiCell, bool Force)
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
  m_DocumentUpdatedSignal();
  return true;
}

bool Document::Translate(Address const& rAddr, TOffset& rRawOffset) const
{
  MemoryArea const* pMemoryArea = GetMemoryArea(rAddr);
  if (pMemoryArea == nullptr)
    return false;

  return pMemoryArea->Translate(rAddr.GetOffset(), rRawOffset);
}

bool Document::Convert(Address const& rAddr, TOffset& rMemAreaOffset) const
{
  MemoryArea const* pMemoryArea = GetMemoryArea(rAddr);
  if (pMemoryArea == nullptr)
    return false;

  return pMemoryArea->Convert(rAddr.GetOffset(), rMemAreaOffset);
}

void Document::AddMemoryArea(MemoryArea* pMemoryArea)
{
  m_MemoryAreas.insert(pMemoryArea);
}

bool Document::IsPresent(Address const& Addr) const
{
  for (MemoryAreaSetType::const_iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsPresent(Addr))
      return true;

  return false;
}

bool Document::Read(Address const& rAddress, void* pBuffer, u32 Size) const
{
  MemoryArea const* pMemoryArea = GetMemoryArea(rAddress);
  if (pMemoryArea == nullptr)
    return false;
  return pMemoryArea->Read(rAddress.GetOffset(), pBuffer, Size);
}

bool Document::Write(Address const& rAddress, void const* pBuffer, u32 Size)
{
  MemoryArea* pMemoryArea = GetMemoryArea(rAddress);
  if (pMemoryArea == nullptr)
    return false;
  return pMemoryArea->Write(rAddress.GetOffset(), pBuffer, Size);
}

u32 Document::GetNumberOfAddress(void) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  u32 Res = 0;
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
    Res += static_cast<u32>((*itMemArea)->GetSize());
  return Res;
}

bool Document::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  if (Offset < 0)
    return MoveAddressBackward(rAddress, rMovedAddress, Offset);
  if (Offset > 0)
    return MoveAddressForward(rAddress, rMovedAddress, Offset);
  rMovedAddress = rAddress;
  return true;
}

bool Document::MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
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

bool Document::MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
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

bool Document::GetNextAddress(Address const& rAddress, Address& rNextAddress) const
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

bool Document::GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const
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

bool Document::ConvertPositionToAddress(u64 Position, Address& rAddress) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  Address FirstAddress = (*m_MemoryAreas.begin())->GetVirtualBase();
  return MoveAddressForward(FirstAddress, rAddress, Position);
}

bool Document::ConvertAddressToPosition(Address const& rAddress, u64& rPosition) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);

  auto Addr = MakeAddress(rAddress.GetBase(), rAddress.GetOffset());

  rPosition = 0;
  auto itMemArea = std::begin(m_MemoryAreas);
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsPresent(Addr))
      break;
    rPosition += (*itMemArea)->GetSize();
  }

  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  auto itCell = (*itMemArea)->Begin();
  auto itEndCell = (*itMemArea)->End();
  auto Offset = Addr.GetOffset();
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

void Document::FindFunctionAddressFromAddress(Address::List& rFunctionAddress, Address const& rAddress) const
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

Address Document::GetTheLastAddressAccessed(void) const
{
 return m_LastAddressAccessed;
}

void Document::SetTheLastAddressAccessed(Address const& rAddr)
{
  m_LastAddressAccessed = rAddr;
}

MEDUSA_NAMESPACE_END
