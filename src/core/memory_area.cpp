#include "medusa/memory_area.hpp"

#include "medusa/value.hpp"
#include "medusa/instruction.hpp"
#include "medusa/character.hpp"

MEDUSA_NAMESPACE_BEGIN

MemoryArea::~MemoryArea(void)
{
  for (TIterator It = m_Cells.begin(); It != m_Cells.end(); ++It)
    delete It->second;

  m_BinStrm.Close();
}

bool MemoryArea::GetNextAddress(Address const& rAddress, Address& rNextAddress) const
{
  TOffset LimitOffset = m_VirtualBase.GetOffset() + GetSize();

  for (auto Offset = rAddress.GetOffset() + 1; Offset < LimitOffset; ++Offset)
  {
    auto pCurrentCell = GetCell(Offset);
    if (pCurrentCell != nullptr)
    {
      rNextAddress = MakeAddress(Offset);
      return true;
    }
  }

  return false;
}

bool MemoryArea::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset < 0)
    return MoveAddressBackward(rAddress, rMovedAddress, Offset);
  else if (Offset > 0)
    return MoveAddressForward(rAddress, rMovedAddress, Offset);
  rMovedAddress = rAddress;
  return true;
}

bool MemoryArea::MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset == 0)
  {
    rMovedAddress = rAddress;
    return true;
  }

  TOffset MovedOffset = rAddress.GetOffset();
  while (Offset++)
  {
    while (true)
    {
      MovedOffset--;
      Cell const* pCurrentCell = GetCell(MovedOffset);
      if (pCurrentCell != nullptr)
        break;
      if (MovedOffset < m_VirtualBase.GetOffset())
        return false;
    }
  }

  rMovedAddress = MakeAddress(MovedOffset);
  return true;
}

bool MemoryArea::MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset == 0)
  {
    rMovedAddress = rAddress;
    return true;
  }

  TOffset MovedOffset = rAddress.GetOffset();
  while (Offset--)
  {
    while (true)
    {
      MovedOffset++;
      Cell const* pCurrentCell = GetCell(MovedOffset);
      if (pCurrentCell != nullptr)
        break;
      if (MovedOffset > (m_VirtualBase.GetOffset() + GetSize()))
        return false;
    }
  }

  rMovedAddress = MakeAddress(MovedOffset);
  return true;
}

bool MemoryArea::GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const
{
  auto Offset = rAddress.GetOffset();

  if (Offset < m_VirtualBase.GetOffset())
  {
    rNearestAddress = m_VirtualBase;
    return true;
  }

  if (GetCell(Offset) != nullptr)
  {
    rNearestAddress = rAddress;
    return true;
  }

  bool Found = false;

  do
  {
    // Avoid integer underflow
    if (Offset == 0x0)
      return false;

    --Offset;

    if (GetCell(Offset) != nullptr)
    {
      Found = true;
      break;
    }
  }
  while (Offset >= m_VirtualBase.GetOffset());

  if (Found == false)
    return false;

  rNearestAddress = MakeAddress(Offset);
  return true;
}

bool MemoryArea::CompareByVirtualBase(MemoryArea const* lhs, MemoryArea const* rhs)
{
  return lhs->GetVirtualBase() < rhs->GetVirtualBase();
}

std::string MemoryArea::ToString(void) const
{
  std::ostringstream oss;

  oss << ";; \"" << m_Name << "\"";
  if (m_VirtualBase.GetAddressingType() != Address::UnknownType)
    oss << " (" << m_VirtualBase.ToString() << " - " << (m_VirtualBase + m_VirtualSize).ToString() << ")";

  return oss.str();
}

void MemoryArea::CreateUnitializeCell(u32 DefaultValueType)
{
  m_Cells.resize(m_BinStrm.GetSize());

  TOffset CurOff = m_VirtualBase.GetOffset();

  for (TIterator It = Begin(); It != End(); ++It)
  {
    It->first = CurOff++;
    It->second = new Value(DefaultValueType);
  }
}

Cell* MemoryArea::GetCell(TOffset Off)
{
  if (IsPresent(Off) == false)
    return nullptr;
  return m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second;
}

Cell const* MemoryArea::GetCell(TOffset Off) const
{
  if (IsPresent(Off) == false)
    return nullptr;
  return m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second;
}

bool MemoryArea::SetCell(TOffset Off, Cell* pCell)
{
  if (IsPresent(Off) == false)
    return false;

  m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second = pCell;
  return true;
}

bool MemoryArea::FillCell(TOffset Off)
{
  Cell* pCell;

  if ((pCell = GetCell(Off)) != nullptr)
    return false;

  return SetCell(Off, new Value(VT_HEX | VS_8BIT));
}

bool MemoryArea::EraseCell(TOffset Off)
{
  if (IsPresent(Off) == false)
    return false;
  // NOTE: Since delete(void*) check if the parameter is nullptr or not, we don't have to
  delete m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second;
  m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second = nullptr;
  return true;
}

void MemoryArea::Sanitize(TOffset Off, size_t OldCellSize, Address::List& rErasedCell)
{
  Cell* pCell = m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second;

  if (pCell == nullptr) return;

  // Clean if needed the previous entry
  TOffset PreviousOff = Off;
  Cell* pPreviousCell = nullptr;
  if (GetPreviousCell(PreviousOff, pPreviousCell) && pPreviousCell != nullptr)
      if (pPreviousCell->GetLength() + PreviousOff > Off)
      {
        EraseCell(PreviousOff);
        rErasedCell.push_back(
          Address(
          m_VirtualBase.GetAddressingType(),
          m_VirtualBase.GetBase(), PreviousOff,
          m_VirtualBase.GetBaseSize(), m_VirtualBase.GetOffsetSize()));
      }

  // Clean if needed the next entry
  size_t CellMaxLen = OldCellSize;
  for (size_t CellLen = 1; CellLen < CellMaxLen; ++CellLen)
  {
    Cell* pCurCell;
    if ((pCurCell = GetCell(Off + CellLen)))
      if (pCurCell->GetLength() + CellLen > CellMaxLen)
        CellMaxLen = pCurCell->GetLength() + CellLen;

    EraseCell(Off + CellLen);
    rErasedCell.push_back(MakeAddress(Off + CellLen));

    // If the deleted cell contained data, we fill the gap with Value
    if (CellLen >= pCell->GetLength())
      FillCell(Off + CellLen);
  }
}

bool MemoryArea::GetPreviousCell(TOffset& rOff, Cell*& prCell)
{
  prCell = nullptr;

  do
  {
    // Avoid integer underflow
    if (rOff == 0x0)
      return false;

    rOff--;

    Cell* pCurrentCell = GetCell(rOff);
    if (pCurrentCell != nullptr)
    {
      prCell = pCurrentCell;
      return true;
    }
  }
  while (rOff >= m_VirtualBase.GetOffset());

  return false;
}

bool MemoryArea::GetNextCell(TOffset& rOff, Cell*& prCell, size_t LimitSize)
{
  prCell = nullptr;

  do
  {
    // Avoid integer overflow (since TOffset is unsigned, -1 means the maximum value)
    if (rOff == -1)
      return false;

    if (LimitSize == 0x0)
      return false;

    rOff++;

    Cell* pCurrentCell = GetCell(rOff);
    if (pCurrentCell != nullptr)
    {
      prCell = pCurrentCell;
      return true;
    }

    LimitSize--;
  }
  while (rOff < m_VirtualBase.GetOffset() + GetSize());

  return false;
}

bool MemoryArea::InsertCell(TOffset Off, Cell* pCell, Address::List& rDeletedCell, bool Force, bool Safe)
{
  boost::lock_guard<MutexType> Lock(m_Mutex);
  size_t OldCellSize = 0;

  if (IsPresent(Off) == false)
    return false;

  auto pOldCell = GetCell(Off);
  // Is there already an allocated cell ?
  if (pOldCell != nullptr)
  {
    OldCellSize = pOldCell->GetLength();

    // If we can't remove it, we return
    if (Force == false)
      return false;

    delete pOldCell;
  }
  m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second = pCell;
  rDeletedCell.push_back(
    Address(
    m_VirtualBase.GetAddressingType(),
    m_VirtualBase.GetBase(), Off,
    m_VirtualBase.GetBaseSize(), m_VirtualBase.GetOffsetSize()));

  if (Safe == true)
    Sanitize(Off, std::max(pCell->GetLength(), OldCellSize), rDeletedCell);

  return true;
}

Cell* MemoryArea::RetrieveCell(TOffset Off)
{
  boost::lock_guard<MutexType> Lock(m_Mutex);
  return GetCell(Off);
}

Cell const* MemoryArea::RetrieveCell(TOffset Off) const
{
  boost::lock_guard<MutexType> Lock(m_Mutex);
  return GetCell(Off);
}

bool MemoryArea::Translate(TOffset VirtualOffset, TOffset& rPhysicalOffset) const
{
  if (IsPresent(VirtualOffset) == false)
    return false;

  rPhysicalOffset = m_PhysicalBase.GetOffset() + (VirtualOffset - m_VirtualBase.GetOffset());
  return true;
}

bool MemoryArea::Convert(TOffset VirtualOffset, TOffset& rMemAreaOffset) const
{
  if (IsPresent(VirtualOffset) == false)
    return false;

  rMemAreaOffset = VirtualOffset - m_VirtualBase.GetOffset();
  return true;
}

/* Physical */

bool PhysicalMemoryArea::Read(TOffset Offset, void* pBuffer, u32 Size) const
{
  if (Offset < m_PhysicalBase.GetOffset())  return false;
  if (Size > m_PhysicalSize)                return false;

  m_BinStrm.Read(Offset - m_PhysicalBase.GetOffset(), pBuffer, Size);
  return true;
}

bool PhysicalMemoryArea::Write(TOffset Offset, void const* pBuffer, u32 Size)
{
  if (Offset < m_PhysicalBase.GetOffset())  return false;
  if (Size > m_PhysicalSize)                return false;

  m_BinStrm.Write(Offset - m_PhysicalBase.GetOffset(), pBuffer, Size);
  return true;
}

/* Mapped */

bool MappedMemoryArea::Read(TOffset Offset, void* pBuffer, u32 Size) const
{
  if (Offset < m_VirtualBase.GetOffset()) return false;
  if (Size > m_VirtualSize)               return false;

  m_BinStrm.Read(Offset - m_VirtualBase.GetOffset(), pBuffer, Size);
  return true;
}

bool MappedMemoryArea::Write(TOffset Offset, void const* pBuffer, u32 Size)
{
  if (Offset < m_VirtualBase.GetOffset()) return false;
  if (Size > m_VirtualSize)               return false;

  m_BinStrm.Write(Offset - m_VirtualBase.GetOffset(), pBuffer, Size);
  return true;
}

/* Virtual */

bool VirtualMemoryArea::Read(TOffset Offset, void* pBuffer, u32 Size) const
{
  if (Offset < m_VirtualBase.GetOffset()) return false;
  if (Size > m_PhysicalSize)              return false;

  m_BinStrm.Read(Offset - m_VirtualBase.GetOffset(), pBuffer, Size);
  return true;
}

bool VirtualMemoryArea::Write(TOffset Offset, void const* pBuffer, u32 Size)
{
  if (Offset < m_VirtualBase.GetOffset())  return false;
  if (Size > m_VirtualSize)                return false;

  m_BinStrm.Write(Offset - m_VirtualBase.GetOffset(), pBuffer, Size);
  return true;
}

MEDUSA_NAMESPACE_END
