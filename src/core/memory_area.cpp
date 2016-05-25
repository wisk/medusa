#include "medusa/memory_area.hpp"

#include "medusa/value.hpp"
#include "medusa/instruction.hpp"
#include "medusa/character.hpp"

#include <boost/format.hpp>

MEDUSA_NAMESPACE_BEGIN

MemoryArea::MemoryArea(
  std::string const& rName,
  u32 Access,
  Tag DefaultArchitectureTag,
  u8 DefaultArchitectureMode)
  : m_Name(rName)
  , m_Access(Access)
  , m_DefaultArchitectureTag(DefaultArchitectureTag)
  , m_DefaultArchitectureMode(DefaultArchitectureMode)
{
}

MemoryArea::~MemoryArea(void)
{
}

void                       MemoryArea::SetDefaultArchitectureTag(Tag arch)
{
  m_DefaultArchitectureTag = arch;
}

void                       MemoryArea::SetDefaultArchitectureMode(u8 mode)
{
  m_DefaultArchitectureMode = mode;
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
  oss << "ma(m " << m_Name << " " << m_FileOffset << " " << m_FileSize << " " << m_VirtualBase.Dump() << " " << m_VirtualSize << " " << Buf << ")";
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

TOffset MappedMemoryArea::GetFileOffset(void) const
{
  return m_FileOffset;
}

u32 MappedMemoryArea::GetFileSize(void) const
{
  return m_FileSize;
}

CellData::SPType MappedMemoryArea::GetCellData(TOffset Offset) const
{
  if (!IsCellPresent(Offset))
    return nullptr;
  size_t CellOff = static_cast<size_t>(Offset - m_VirtualBase.GetOffset());
  if (CellOff >= m_Cells.size()) {
    return std::make_shared<CellData>(Cell::ValueType, ValueDetail::HexadecimalType, 1);
  }

  auto spCellData = m_Cells[CellOff];
  if (spCellData == nullptr)
    return std::make_shared<CellData>(Cell::ValueType, ValueDetail::HexadecimalType, 1);

  return spCellData;
}

// TODO: check cell boundary
bool MappedMemoryArea::SetCellData(TOffset Offset, CellData::SPType spCellData, Address::List& rDeletedCellAddresses, bool Force)
{
  if (spCellData == nullptr)
    return _RemoveCell(Offset, spCellData);

  return _InsertCell(Offset, spCellData);
}

bool MappedMemoryArea::SetCellData(TOffset Offset, CellData::SPType spCellData)
{
   if (spCellData == nullptr)
     return _RemoveCell(Offset, spCellData);

   return _InsertCell(Offset, spCellData);
}

void MappedMemoryArea::ForEachCellData(CellDataPredicat Predicat) const
{
  TOffset CurOff = 0x0;
  std::for_each(std::begin(m_Cells), std::end(m_Cells), [&CurOff, &Predicat](CellData::SPType spCellData)
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

// TODO: Check if this function works for every cases
bool MappedMemoryArea::MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset == 0)
  {
    rMovedAddress = rAddress;
    return true;
  }

  Offset = -Offset;

  TOffset MovedOff      = rAddress.GetOffset();
  TOffset PrevOff;
  TOffset MemAreaBegOff = m_VirtualBase.GetOffset();
  CellData::SPType spLastCellData;

  for (PrevOff = MovedOff - 1; PrevOff >= MemAreaBegOff; --PrevOff)
  {
    auto spCellData = GetCellData(PrevOff);
    if (spCellData == spLastCellData)
      continue;
    spLastCellData = spCellData;

    --Offset;
    if (Offset == 0)
    {
      if (PrevOff == 0)
      {
        rMovedAddress = MakeAddress(0x0);
        return true;
      }

      while (GetCellData(PrevOff - 1) == spCellData)
      {
        --PrevOff;

        if (PrevOff == 0)
        {
          rMovedAddress = MakeAddress(0x0);
          return true;
        }
      }
      rMovedAddress = MakeAddress(PrevOff);
      return true;
    }
  }

  return false;
}

bool MappedMemoryArea::MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset == 0)
  {
    rMovedAddress = rAddress;
    return true;
  }

  TOffset MovedOffset = rAddress.GetOffset();
  for (; Offset; --Offset)
  {
    while (true)
    {
      auto spCellData = GetCellData(MovedOffset);
      if (spCellData != nullptr)
      {
        // TODO: check intoverflow here
        // Check intoverflow
        auto CellDataLength = spCellData->GetLength();
        auto OffsetAfterMove = MovedOffset + spCellData->GetLength();
	bool is_ovf = (MovedOffset > 0 && CellDataLength > 0 && OffsetAfterMove < 0)
	  || (MovedOffset < 0 && CellDataLength < 0 && OffsetAfterMove > 0);

	if (!is_ovf)
	  MovedOffset += spCellData->GetLength();
      }
      else
        ++MovedOffset;
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

bool MappedMemoryArea::_InsertCell(TOffset Offset, CellData::SPType spCellData)
{
  if (!IsCellPresent(Offset))
    return false;

  size_t CellOffset = static_cast<size_t>(Offset - m_VirtualBase.GetOffset());

  size_t OldSize = m_Cells.size();
  size_t NewSize = CellOffset + spCellData->GetLength();

  // Grow cells container if required
  if (OldSize < NewSize)
  {
    m_Cells.resize(NewSize);

    // LATER(KS): Is it really necessary?
    // A resize initialize element using
    // default constructor or with the zero value
    // for primitive types
  }

  u16 CellLen = spCellData->GetLength();
  for (u16 i = 0; i < CellLen; ++i)
  {
    m_Cells[CellOffset + i] = spCellData;
  }

  return true;
}

bool MappedMemoryArea::_RemoveCell(TOffset Offset, CellData::SPType spCellData)
{
  if (!IsCellPresent(Offset))
    return false;

  // LATER(KS): Optimize this piece of code
  size_t CellOffset = static_cast<size_t>(Offset - m_VirtualBase.GetOffset());
  if (spCellData == nullptr)
    spCellData = m_Cells[CellOffset];
  if (spCellData == nullptr)
    return true;
  u16 CellLen = spCellData->GetLength();
  for (u16 i = 0; i < CellLen; ++i)
  {
    if (IsCellPresent(CellOffset - i))
    {
      auto spPrevCellData = m_Cells[CellOffset - i];
      if (spPrevCellData == spCellData)
        m_Cells[CellOffset - i] = nullptr;
    }

    if (IsCellPresent(Offset + i))
    {
      if (CellOffset + i >= m_Cells.size())
        return false;
      auto spNextCellData = m_Cells[CellOffset + i];
      if (spNextCellData == spCellData)
        m_Cells[CellOffset + i] = nullptr;
    }
  }

  return true;
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
  oss << "ma(v " << m_Name << " " << m_VirtualBase.Dump() << " " << m_VirtualSize << " " << Buf << ")";
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

TOffset VirtualMemoryArea::GetFileOffset(void) const
{
  return 0;
}

u32 VirtualMemoryArea::GetFileSize(void) const
{
  return 0;
}

bool VirtualMemoryArea::SetCellData(TOffset, CellData::SPType)
{
 return false;
}

CellData::SPType VirtualMemoryArea::GetCellData(TOffset Offset) const
{
  if (IsCellPresent(Offset) == false)
    return nullptr;
  return std::make_shared<CellData>(Cell::ValueType, ValueDetail::HexadecimalType, 1, MEDUSA_ARCH_UNK);
}

bool VirtualMemoryArea::SetCellData(TOffset Offset, CellData::SPType spCell, Address::List& rDeletedCellAddresses, bool Force)
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
