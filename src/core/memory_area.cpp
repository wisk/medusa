#include "medusa/memory_area.hpp"

#include "medusa/value.hpp"
#include "medusa/instruction.hpp"
#include "medusa/character.hpp"

#include <boost/format.hpp>

MEDUSA_NAMESPACE_BEGIN

u32 MappedMemoryArea::GetSize(void) const
{
  return m_VirtualSize;
}

std::string MappedMemoryArea::ToString(void) const
{
  return (boost::format("; mapped memory area %s %s %#08x") % m_Name % m_VirtualBase.ToString() % m_VirtualSize).str();
}

Cell::SPtr MappedMemoryArea::GetCell(TOffset Offset) const
{
  if (IsCellPresent(Offset) == false)
    return Cell::SPtr();

  size_t CellOff = static_cast<size_t>(Offset - m_VirtualBase.GetOffset());
  if (CellOff >= m_Cells.size())
    return std::make_shared<Value>(VT_HEX | VS_8BIT);

  auto spCell = m_Cells[CellOff].second;
  if (spCell == nullptr)
  {
    TOffset PrevOff;
    if (_GetPreviousCellOffset(CellOff, PrevOff))
    {
      if (CellOff < PrevOff + m_Cells[PrevOff].second->GetLength())
        return Cell::SPtr();
    }
    return std::make_shared<Value>(VT_HEX | VS_8BIT);
  }

  return spCell;
}

// TODO: check cell boundary
bool MappedMemoryArea::SetCell(TOffset Offset, Cell::SPtr spCell, Address::List& rDeletedCellAddresses, bool Force)
{
  if (IsCellPresent(Offset) == false)
    return false;

  size_t CellOffset = static_cast<size_t>(Offset - m_VirtualBase.GetOffset());
  size_t OldSize = m_Cells.size();
  size_t NewSize = CellOffset + spCell->GetLength();

  if (OldSize < NewSize)
  {
    m_Cells.resize(NewSize);

    for (size_t Idx = OldSize; Idx < NewSize; ++Idx)
      m_Cells[Idx].first = m_VirtualBase.GetOffset() + OldSize + Idx;
  }

  m_Cells[CellOffset].second = spCell;
  ++CellOffset;
  for (; CellOffset < NewSize; ++CellOffset)
    m_Cells[CellOffset].second = nullptr;
  return true;
}

bool MappedMemoryArea::IsCellPresent(TOffset Offset) const
{
  return m_VirtualBase.IsBetween(m_VirtualSize, Offset);
}

Address MappedMemoryArea::GetBaseAddress(void) const
{
  return m_VirtualBase;
}

Address MappedMemoryArea::MakeAddress(TOffset Offset) const
{
  Address Addr = m_VirtualBase;
  Addr.SetOffset(Offset);
  return Addr;
}

bool MappedMemoryArea::GetNextAddress(Address const& rAddress, Address& rNextAddress) const
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

bool MappedMemoryArea::GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const
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

bool MappedMemoryArea::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset < 0)
    return MoveAddressBackward(rAddress, rMovedAddress, Offset);
  else if (Offset > 0)
    return MoveAddressForward(rAddress, rMovedAddress, Offset);
  rMovedAddress = rAddress;
  return true;
}

bool MappedMemoryArea::MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
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
      // We must avoid underflow
      if (MovedOffset == 0)
        return false;
      MovedOffset--;

      if (IsCellPresent(MovedOffset))
        break;
      if (MovedOffset < m_VirtualBase.GetOffset())
        return false;
    }
  }

  rMovedAddress = MakeAddress(MovedOffset);
  return true;
}

bool MappedMemoryArea::MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
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
      auto spCell = GetCell(MovedOffset);
      if (spCell != nullptr)
        MovedOffset += spCell->GetLength();
      else
        MovedOffset++;
      if (IsCellPresent(MovedOffset))
        break;
      if (MovedOffset > (m_VirtualBase.GetOffset() + GetSize()))
        return false;
    }
  }

  rMovedAddress = MakeAddress(MovedOffset);
  return true;
}

bool MappedMemoryArea::ConvertOffsetToPosition(TOffset Offset, u64& rPosition) const
{
  auto itCell = std::begin(m_Cells);
  auto itEndCell = std::end(m_Cells);
  for (; itCell != itEndCell; ++itCell)
  {
    if (itCell->first >= Offset)
    {
      if (itCell->first != Offset)
        --rPosition;
      break;
    }
    ++rPosition;
  }
  
  return itCell != itEndCell;
}

bool MappedMemoryArea::ConvertOffsetToFileOffset(TOffset Offset, TOffset& rFileOffset) const
{
  if (IsCellPresent(Offset) == false)
    return false;
  rFileOffset = (Offset - m_VirtualBase.GetOffset()) + m_FileOffset;
  return true;
}

bool MappedMemoryArea::_GetPreviousCellOffset(TOffset Offset, TOffset& rPreviousOffset) const
{
  u32 Count = 0;
  while (Offset != 0x0)
  {
    --Offset;
    if (Count++ > 0x20)
      return false;
    if (m_Cells[Offset].second != nullptr)
    {
      rPreviousOffset = Offset;
      return true;
    }
  }

  return false;
}

u32 VirtualMemoryArea::GetSize(void) const
{
  return m_VirtualSize;
}

std::string VirtualMemoryArea::ToString(void) const
{
  return (boost::format("; virtual memory area %s %s %#08x") % m_Name % m_VirtualBase.ToString() % m_VirtualSize).str();
}

Cell::SPtr VirtualMemoryArea::GetCell(TOffset Offset) const
{
  if (IsCellPresent(Offset) == false)
    return nullptr;
  return std::make_shared<Value>(VT_HEX | VS_8BIT);
}

bool VirtualMemoryArea::SetCell(TOffset Offset, Cell::SPtr spCell, Address::List& rDeletedCellAddresses, bool Force)
{
  return false;
}

bool VirtualMemoryArea::IsCellPresent(TOffset Offset) const
{
  return m_VirtualBase.IsBetween(GetSize(), Offset);
}

Address VirtualMemoryArea::GetBaseAddress(void) const
{
  return m_VirtualBase;
}

Address VirtualMemoryArea::MakeAddress(TOffset Offset) const
{
  Address Addr = m_VirtualBase;
  Addr.SetOffset(Offset);
  return Addr;
}

bool VirtualMemoryArea::GetNextAddress(Address const& rAddress, Address& rNextAddress) const
{
  TOffset Offset = rAddress.GetOffset() + 1;
  if (IsCellPresent(Offset) == false)
    return false;
  rNextAddress = MakeAddress(Offset);
  return true;
}

bool VirtualMemoryArea::GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const
{
  if (IsCellPresent(rAddress.GetOffset()) == false)
    return false;

  rNearestAddress = rAddress;
  return true;
}

bool VirtualMemoryArea::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset < 0)
    return MoveAddressBackward(rAddress, rMovedAddress, Offset);
  else if (Offset > 0)
    return MoveAddressForward(rAddress, rMovedAddress, Offset);

  if (IsCellPresent(rAddress.GetOffset()) == false)
    return false;
  rMovedAddress = rAddress;
  return true;
}

bool VirtualMemoryArea::MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  TOffset MovedOffset = rAddress.GetOffset() + Offset;
  if (IsCellPresent(MovedOffset) == false)
    return false;
  rMovedAddress = MakeAddress(MovedOffset);
  return true;
}

bool VirtualMemoryArea::MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  TOffset MovedOffset = rAddress.GetOffset() + Offset;
  if (IsCellPresent(MovedOffset) == false)
    return false;
  rMovedAddress = MakeAddress(MovedOffset);
  return true;
}

bool VirtualMemoryArea::ConvertOffsetToPosition(TOffset Offset, u64& rPosition) const
{
  if (IsCellPresent(Offset) == false)
    return false;

  rPosition = Offset - m_VirtualBase.GetOffset();
  return true;
}

bool VirtualMemoryArea::ConvertOffsetToFileOffset(TOffset Offset, TOffset& rFileOffset) const
{
  return false;
}

MEDUSA_NAMESPACE_END
