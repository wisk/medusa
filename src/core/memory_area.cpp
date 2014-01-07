#include "medusa/memory_area.hpp"

#include "medusa/value.hpp"
#include "medusa/instruction.hpp"
#include "medusa/character.hpp"

#include <boost/format.hpp>

MEDUSA_NAMESPACE_BEGIN

MemoryArea::~MemoryArea(void)
{
}

MappedMemoryArea::~MappedMemoryArea(void)
{
  m_Cells.clear();
}

u32 MappedMemoryArea::GetSize(void) const
{
  return m_VirtualSize;
}

std::string MappedMemoryArea::Dump(void) const
{
  char Buf[4];
  Buf[0] = (m_Access & MemoryArea::Read)    ? 'R' : '-';
  Buf[1] = (m_Access & MemoryArea::Write)   ? 'W' : '-';
  Buf[2] = (m_Access & MemoryArea::Execute) ? 'X' : '-';
  Buf[3] = '\0';

  std::ostringstream oss;
  oss << std::hex << std::showbase;
  oss << "ma(m, " << m_FileOffset << ", " << m_FileSize << ", " << m_VirtualBase.Dump() << m_VirtualSize << ")";
  return oss.str();
}

std::string MappedMemoryArea::ToString(void) const
{
  char Buf[4];
  Buf[0] = (m_Access & MemoryArea::Read)    ? 'R' : '-';
  Buf[1] = (m_Access & MemoryArea::Write)   ? 'W' : '-';
  Buf[2] = (m_Access & MemoryArea::Execute) ? 'X' : '-';
  Buf[3] = '\0';
  return (boost::format("; mapped memory area %s %s %#08x %s") % m_Name % m_VirtualBase.ToString() % m_VirtualSize % Buf).str();
}

CellData::SPtr MappedMemoryArea::GetCellData(TOffset Offset) const
{
  if (IsCellPresent(Offset) == false)
    return CellData::SPtr();

  size_t CellOff = static_cast<size_t>(Offset - m_VirtualBase.GetOffset());
  if (CellOff >= m_Cells.size())
    return std::make_shared<CellData>(Cell::ValueType, Value::HexadecimalType, 1);

  auto spCellData = m_Cells[CellOff];
  if (spCellData == nullptr)
  {
    TOffset PrevOff;
    if (_GetPreviousCellOffset(CellOff, PrevOff))
    {
      if (CellOff < PrevOff + m_Cells[PrevOff]->GetLength())
        return CellData::SPtr();
    }
    return std::make_shared<CellData>(Cell::ValueType, Value::HexadecimalType, 1);
  }

  return spCellData;
}

// TODO: check cell boundary
bool MappedMemoryArea::SetCellData(TOffset Offset, CellData::SPtr spCellData, Address::List& rDeletedCellAddresses, bool Force)
{
  if (IsCellPresent(Offset) == false)
    return false;

  size_t CellOffset = static_cast<size_t>(Offset - m_VirtualBase.GetOffset());
  size_t OldSize = m_Cells.size();
  size_t NewSize = CellOffset + spCellData->GetLength();

  if (OldSize < NewSize)
  {
    m_Cells.resize(NewSize);

    for (size_t Idx = OldSize; Idx < NewSize; ++Idx)
    {
      m_Cells[Idx] = nullptr;
    }
  }

  m_Cells[CellOffset] = spCellData;
  ++CellOffset;
  for (; CellOffset < NewSize; ++CellOffset)
    m_Cells[CellOffset] = nullptr;
  return true;
}

void MappedMemoryArea::ForEachCellData(CellDataPredicat Predicat) const
{
  auto itCellDataEnd = std::end(m_Cells);
  TOffset CurOff = 0x0;
  std::for_each(std::begin(m_Cells), std::end(m_Cells), [&CurOff, &Predicat](CellData::SPtr spCellData)
  {
    if (spCellData)
      Predicat(CurOff, spCellData);
    ++CurOff;
  });
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
    auto pCurrentCell = GetCellData(Offset);
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

  if (GetCellData(Offset) != nullptr)
  {
    rNearestAddress = MakeAddress(rAddress.GetOffset());
    return true;
  }

  bool Found = false;

  do
  {
    // Avoid integer underflow
    if (Offset == 0x0)
      return false;

    --Offset;

    if (GetCellData(Offset) != nullptr)
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
      auto spCellData = GetCellData(MovedOffset);
      if (spCellData != nullptr)
        MovedOffset += spCellData->GetLength();
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
  TOffset CurOff = m_VirtualBase.GetOffset();
  for (; itCell != itEndCell; ++itCell)
  {
    if (CurOff >= Offset)
    {
      if (CurOff != Offset)
        --rPosition;
      break;
    }
    ++rPosition;
    ++CurOff;
  }

  return itCell != itEndCell;
}

bool MappedMemoryArea::ConvertOffsetToFileOffset(TOffset Offset, TOffset& rFileOffset) const
{
  if (m_VirtualBase.IsBetween(m_FileSize, Offset) == false)
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
    if (m_Cells[Offset] != nullptr)
    {
      rPreviousOffset = Offset;
      return true;
    }
  }

  return false;
}

VirtualMemoryArea::~VirtualMemoryArea(void)
{
}

std::string VirtualMemoryArea::Dump(void) const
{
  char Buf[4];
  Buf[0] = (m_Access & MemoryArea::Read)    ? 'R' : '-';
  Buf[1] = (m_Access & MemoryArea::Write)   ? 'W' : '-';
  Buf[2] = (m_Access & MemoryArea::Execute) ? 'X' : '-';
  Buf[3] = '\0';

  std::ostringstream oss;
  oss << std::hex << std::showbase;
  oss << "ma(v, " << m_VirtualBase.Dump() << m_VirtualSize << ")";
  return oss.str();
}

u32 VirtualMemoryArea::GetSize(void) const
{
  return m_VirtualSize;
}

std::string VirtualMemoryArea::ToString(void) const
{
  char Buf[4];
  Buf[0] = (m_Access & MemoryArea::Read)    ? 'R' : '-';
  Buf[1] = (m_Access & MemoryArea::Write)   ? 'W' : '-';
  Buf[2] = (m_Access & MemoryArea::Execute) ? 'X' : '-';
  Buf[3] = '\0';
  return (boost::format("; virtual memory area %s %s %#08x %s") % m_Name % m_VirtualBase.ToString() % m_VirtualSize % Buf).str();
}

CellData::SPtr VirtualMemoryArea::GetCellData(TOffset Offset) const
{
  if (IsCellPresent(Offset) == false)
    return nullptr;
  return std::make_shared<CellData>(Cell::ValueType, Value::HexadecimalType, 1, MEDUSA_ARCH_UNK);
}

bool VirtualMemoryArea::SetCellData(TOffset Offset, CellData::SPtr spCell, Address::List& rDeletedCellAddresses, bool Force)
{
  return false;
}

void VirtualMemoryArea::ForEachCellData(CellDataPredicat) const
{
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

  rNearestAddress = MakeAddress(rAddress.GetOffset());
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
