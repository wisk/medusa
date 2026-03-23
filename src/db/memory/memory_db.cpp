#include "memory_db.hpp"

#include <medusa/module.hpp>
#include <medusa/log.hpp>
#include <medusa/util.hpp>

MemoryDatabase::MemoryDatabase(void)
  : m_ImageBase(0x0), m_DefaultAddressingType(Address::UnknownType)
{
}

MemoryDatabase::~MemoryDatabase(void)
{
}

bool MemoryDatabase::_ConvertIdToAddress(int Id, OffsetType Offset, u16 Size, Address& rAddress) const
{
  for (auto const& rMemArea : m_MemoryAreas)
  {
    if (rMemArea.GetId() == Id)
    {
      auto& rBaseAddr = rMemArea.GetBaseAddress();
      auto MemAreaSize = rMemArea.GetSize();
      if ((Offset + Size) >= MemAreaSize)
        return false;
      rAddress = rBaseAddr + Offset;
      return true;
    }
  }
  return false;
}

bool MemoryDatabase::_ConvertAddressToId(Address const& rAddress, int& rId, OffsetType& rOffset) const
{
  for (auto const& rMemArea : m_MemoryAreas)
  {
    auto const& rBaseAddr = rMemArea.GetBaseAddress();
    if (rBaseAddr.IsBetween(rMemArea.GetSize(), rAddress))
    {
      rId = rMemArea.GetId();
      rOffset = rAddress.GetOffset() - rBaseAddr.GetOffset();
      return true;
    }
  }
  return false;
}

bool MemoryDatabase::_ConvertAddressToId(Address const& rAddress, int& rId, OffsetType& rOffset, OffsetType& rMemoryAreaOffset, u32& rMemoryAreaSize) const
{
  for (auto const& rMemArea : m_MemoryAreas)
  {
    auto const& rBaseAddr = rMemArea.GetBaseAddress();
    auto const MemAreaSize = rMemArea.GetSize();
    if (rBaseAddr.IsBetween(MemAreaSize, rAddress))
    {
      rId = rMemArea.GetId();
      rOffset = rAddress.GetOffset() - rBaseAddr.GetOffset();
      rMemoryAreaSize = MemAreaSize;
      return true;
    }
  }
  return false;
}

bool MemoryDatabase::_GetNextMemoryAreaId(MemoryArea const& rMemoryArea, MemoryArea& rNextMemoryArea) const
{
  auto itBeforeLastMemArea = std::end(m_MemoryAreas);
  --itBeforeLastMemArea;
  for (auto itMemArea = std::cbegin(m_MemoryAreas); itMemArea != itBeforeLastMemArea; ++itMemArea)
  {
    if (itMemArea->GetId() == rMemoryArea.GetId())
    {
      ++itMemArea;
      rNextMemoryArea = *itMemArea;
      return true;
    }
  }
  return false;
}

bool MemoryDatabase::_GetPreviousMemoryAreaId(MemoryArea const& rMemoryArea, MemoryArea& rPreviousMemoryArea) const
{
  auto itMemArea = std::cbegin(m_MemoryAreas);
  if (itMemArea->GetId() == rMemoryArea.GetId())
    return false;
  ++itMemArea;
  for (; itMemArea != std::cend(m_MemoryAreas); ++itMemArea)
  {
    if (itMemArea->GetId() == rMemoryArea.GetId())
    {
      --itMemArea;
      rPreviousMemoryArea = *itMemArea;
      return true;
    }
  }
  return false;
}

std::string MemoryDatabase::GetName(void) const
{
  return "Memory";
}

std::string MemoryDatabase::GetExtension(void) const
{
  return "";
}

bool MemoryDatabase::IsCompatible(Path const &rDatabasePath) const
{
  return false;
}

bool MemoryDatabase::Open(Path const &rDatabasePath)
{
  return false;
}

bool MemoryDatabase::Create(Path const &rDatabasePath, bool Force)
{
  return true;
}

bool MemoryDatabase::Flush(void)
{
  return true;
}

bool MemoryDatabase::Close(void)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  m_MemoryAreas.clear();
  m_LabelsMap.clear();
  m_ReverseLabelsMap.clear();
  m_CrossReferencesToFrom.clear();
  m_CrossReferencesFromTo.clear();
  m_MultiCellsMap.clear();
  m_CellLayoutMap.clear();
  m_CommentsMap.clear();
  m_StructureDetailsMap.clear();
  m_ValueDetailsMap.clear();
  m_FunctionDetailsMap.clear();
  m_DetailBindingsMap.clear();
  return true;
}

bool MemoryDatabase::RegisterArchitectureTag(Tag ArchitectureTag)
{
  auto itTag = std::find(std::begin(m_ArchitectureTags), std::end(m_ArchitectureTags), ArchitectureTag);
  if (itTag != std::end(m_ArchitectureTags))
    return false;
  m_ArchitectureTags.push_back(ArchitectureTag);
  return true;
}

bool MemoryDatabase::UnregisterArchitectureTag(Tag ArchitectureTag)
{
  auto itTag = std::find(std::begin(m_ArchitectureTags), std::end(m_ArchitectureTags), ArchitectureTag);
  if (itTag == std::end(m_ArchitectureTags))
    return false;
  m_ArchitectureTags.erase(itTag);
  return true;
}

std::vector<Tag> MemoryDatabase::GetArchitectureTags(void) const
{
  return m_ArchitectureTags;
}

bool MemoryDatabase::SetArchitecture(Address const &rAddress, Tag ArchitectureTag, u8 Mode,
  Database::SetArchitectureModeType SetArchMode)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  if (SetArchMode == Database::SetArchitectureModeType::ByMemoryArea)
  {
    // Find the memory area and update its architecture
    for (auto it = m_MemoryAreas.begin(); it != m_MemoryAreas.end(); ++it)
    {
      if (it->GetBaseAddress().IsBetween(it->GetSize(), rAddress))
      {
        auto MemArea = *it;
        m_MemoryAreas.erase(it);
        MemArea.SetArchitectureTag(ArchitectureTag);
        MemArea.SetArchitectureMode(Mode);
        m_MemoryAreas.insert(MemArea);
        return true;
      }
    }
    return false;
  }
  else if (SetArchMode == Database::SetArchitectureModeType::ByCell)
  {
    int Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    auto Key = std::make_pair(Id, Offset);
    auto itCell = m_CellsMap.find(Key);
    if (itCell == m_CellsMap.end())
      return false;
    itCell->second.SetArchitectureTag(ArchitectureTag);
    itCell->second.SetMode(Mode);
    return true;
  }
  return false;
}

bool MemoryDatabase::GetImageBase(ImageBaseType& rImageBase) const
{
  rImageBase = m_ImageBase;
  return true;
}

bool MemoryDatabase::SetImageBase(ImageBaseType ImageBase)
{
  m_ImageBase = ImageBase;
  return true;
}

bool MemoryDatabase::GetMemoryArea(Address const &rAddress, MemoryArea& rMemArea) const
{
  for (auto const& rCurMemArea : m_MemoryAreas)
  {
    auto MemAreaSize = rCurMemArea.GetSize();
    if (rCurMemArea.GetBaseAddress().IsBetween(MemAreaSize, rAddress))
    {
      rMemArea = rCurMemArea;
      return true;
    }
  }
  return false;
}


void MemoryDatabase::ForEachMemoryArea(MemoryAreaCallback Callback) const
{
  for (auto const& rMemArea : m_MemoryAreas)
  {
    Callback(rMemArea);
  }
}

bool MemoryDatabase::AddMemoryArea(MemoryArea const& rMemArea)
{
  static int Id = 0;
  MemoryArea DupMemArea = rMemArea;
  DupMemArea.SetId(Id++);
  m_MemoryAreas.insert(DupMemArea);
  m_CellLayoutMap[DupMemArea.GetId()].resize(DupMemArea.GetSize());
  return true;
}

bool MemoryDatabase::RemoveMemoryArea(MemoryArea const& rMemArea)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  for (auto it = m_MemoryAreas.begin(); it != m_MemoryAreas.end(); ++it)
  {
    if (it->GetId() == rMemArea.GetId())
    {
      m_CellLayoutMap.erase(it->GetId());
      m_MemoryAreas.erase(it);
      return true;
    }
  }
  return false;
}

bool MemoryDatabase::MoveMemoryArea(MemoryArea const& rMemArea, Address const& rBaseAddress)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  for (auto it = m_MemoryAreas.begin(); it != m_MemoryAreas.end(); ++it)
  {
    if (it->GetId() == rMemArea.GetId())
    {
      auto MovedMemArea = *it;
      m_MemoryAreas.erase(it);
      MovedMemArea.SetBaseAddress(rBaseAddress);
      m_MemoryAreas.insert(MovedMemArea);
      return true;
    }
  }
  return false;
}

bool MemoryDatabase::GetDefaultAddressingType(Address::Type& rAddressType) const
{
  rAddressType = m_DefaultAddressingType;
  return true;
}

bool MemoryDatabase::SetDefaultAddressingType(Address::Type AddressType)
{
  m_DefaultAddressingType = AddressType;
  return true;
}

// Implemented possibilities:
// - Logical Address → Linear Address → (Relative Address → Physical address) WIP
// - (Physical Address → Relative Address) → Linear Address → Logical Address (might not normalized)
// 
// See document.hpp Document::Translate
bool MemoryDatabase::TranslateAddress(Address const& rAddress, Address::Type ToConvert, Address& rTranslatedAddress) const
{
  std::lock_guard<std::mutex> Lock(m_Lock);

  for (auto const& rMemArea : m_MemoryAreas)
  {
    if (rMemArea.GetType() != MemoryArea::MappedType && rMemArea.GetType() != MemoryArea::PhysicalType)
      continue;

    auto const& rBaseAddr = rMemArea.GetBaseAddress();

    // Relative → Physical: find mapped area containing the relative address, compute file offset
    if (rAddress.GetAddressingType() == Address::RelativeType && ToConvert == Address::PhysicalType)
    {
      if (rMemArea.GetType() != MemoryArea::MappedType)
        continue;
      if (rBaseAddr.GetAddressingType() != Address::RelativeType)
        continue;
      auto Off = rAddress.GetOffset();
      auto BaseOff = rBaseAddr.GetOffset();
      if (Off >= BaseOff && Off < BaseOff + rMemArea.GetSize())
      {
        rTranslatedAddress = Address(Address::PhysicalType, rMemArea.GetFileOffset() + (Off - BaseOff));
        return true;
      }
    }

    // Physical → Linear: find area by file offset, compute linear address
    if (rAddress.GetAddressingType() == Address::PhysicalType && ToConvert == Address::LinearType)
    {
      if (rMemArea.GetType() != MemoryArea::MappedType)
        continue;
      auto FileOff = rAddress.GetOffset();
      if (FileOff >= rMemArea.GetFileOffset() && FileOff < rMemArea.GetFileOffset() + rMemArea.GetFileSize())
      {
        rTranslatedAddress = Address(Address::LinearType, m_ImageBase + rBaseAddr.GetOffset() + (FileOff - rMemArea.GetFileOffset()));
        return true;
      }
    }
  }

  return false;
}

bool MemoryDatabase::GetFirstAddress(Address &rAddress) const
{
  if (m_MemoryAreas.size() == 0)
    return false;
  rAddress = std::cbegin(m_MemoryAreas)->GetBaseAddress();
  return true;
}

bool MemoryDatabase::GetLastAddress(Address &rAddress) const
{
  if (m_MemoryAreas.size() == 0)
    return false;
  auto itMemArea = std::cend(m_MemoryAreas);
  --itMemArea;
  // FIXME: if the size is 0x0
  rAddress = itMemArea->GetBaseAddress() + (itMemArea->GetSize() - 1);
  return true;
}

bool MemoryDatabase::MoveAddress(Address const &rAddress, Address &rMovedAddress, s64 Displacement) const
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  MemoryArea MemArea;
  if (!GetMemoryArea(rAddress, MemArea))
    return false;

  if (Displacement < 0)
  {
    do
    {
      if (Offset == 0x0)
      {
        if (!_GetPreviousMemoryAreaId(MemArea, MemArea))
          return false;
        if (MemArea.GetSize() < 1)
          return false;
        Id = MemArea.GetId();
        Offset = MemArea.GetSize() - 1;
      }

      auto itCellLayoutPair = m_CellLayoutMap.find(Id);
      if (itCellLayoutPair == std::cend(m_CellLayoutMap))
        return false;
      auto const& rCellLayout = itCellLayoutPair->second;
      if (Offset > rCellLayout.size())
        return false;
      if (rCellLayout[Offset].IsEmpty())
        --Offset;
      else
      {
        auto const& rCurLayout = rCellLayout[Offset];
        if (rCurLayout.GetOffset() == 0x0)
        {
          if (rCurLayout.GetSize() > Offset)
            Offset = 0x0;
          else
            Offset -= rCurLayout.GetSize();
        }
        else
          Offset -= rCurLayout.GetOffset();
      }

    } while (++Displacement);
    return _ConvertIdToAddress(Id, Offset, 0, rMovedAddress);
  }
  else if (Displacement > 0)
  {
    do
    {
      if (Offset >= MemArea.GetSize())
      {
        if (!_GetNextMemoryAreaId(MemArea, MemArea))
          return false;
        if (MemArea.GetSize() < 1)
          return false;
        Id = MemArea.GetId();
        Offset = 0x0;
      }

      auto itCellLayout = m_CellLayoutMap.find(Id);
      if (itCellLayout == std::cend(m_CellLayoutMap))
        return false;
      auto const& rCellLayout = itCellLayout->second;
      if (Offset > rCellLayout.size())
        return false;
      if (rCellLayout[Offset].IsEmpty())
        ++Offset;
      else
        Offset += rCellLayout[Offset].GetNextCellOffset();

    } while (--Displacement);
    return _ConvertIdToAddress(Id, Offset, 0, rMovedAddress);
  }
  // Displacement == 0
  rMovedAddress = rAddress;
  return true;
}

bool MemoryDatabase::ConvertAddressToPosition(Address const &rAddress, u32 &rPosition) const
{
  u32 Pos = 0;
  for (auto const& rMemArea : m_MemoryAreas)
  {
    auto const& rBaseAddr = rMemArea.GetBaseAddress();
    if (rBaseAddr.IsBetween(rMemArea.GetSize(), rAddress))
    {
      rPosition = Pos + static_cast<u32>(rAddress.GetOffset() - rBaseAddr.GetOffset());
      return true;
    }
    Pos += rMemArea.GetSize();
  }
  return false;
}

bool MemoryDatabase::ConvertPositionToAddress(u32 Position, Address &rAddress) const
{
  u32 Pos = 0;
  for (auto const& rMemArea : m_MemoryAreas)
  {
    if (Position < Pos + rMemArea.GetSize())
    {
      rAddress = rMemArea.GetBaseAddress() + (Position - Pos);
      return true;
    }
    Pos += rMemArea.GetSize();
  }
  return false;
}

bool MemoryDatabase::AddLabel(Address const &rAddress, Label const &rLabel)
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto const MemAreaRel = std::make_pair(Id, Offset);
  auto itLbl = m_LabelsMap.find(MemAreaRel);
  if (itLbl != std::end(m_LabelsMap))
    return false;
  m_ReverseLabelsMap[rLabel.GetLabel()] = MemAreaRel;
  m_LabelsMap[MemAreaRel] = rLabel;
  return true;
}

bool MemoryDatabase::RemoveLabel(Address const &rAddress)
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto const MemAreaRel = std::make_pair(Id, Offset);
  auto itLbl = m_LabelsMap.find(MemAreaRel);
  if (itLbl != std::end(m_LabelsMap))
    return false;
  auto const& rLblName = itLbl->second.GetLabel();
  m_ReverseLabelsMap.erase(rLblName);
  m_LabelsMap.erase(MemAreaRel);
  return true;
}

bool MemoryDatabase::GetLabel(Address const &rAddress, Label &rLabel) const
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto const MemAreaRel = std::make_pair(Id, Offset);
  auto itLbl = m_LabelsMap.find(MemAreaRel);
  if (itLbl == std::end(m_LabelsMap))
    return false;
  rLabel = itLbl->second;
  return true;
}

bool MemoryDatabase::GetLabelAddress(Label const &rLabel, Address &rAddress) const
{
  auto const& rLblName = rLabel.GetLabel();
  int Id;
  OffsetType Offset;
  auto itLbl = m_ReverseLabelsMap.find(rLblName);
  if (itLbl == std::cend(m_ReverseLabelsMap))
    return false;
  std::tie(Id, Offset) = itLbl->second;
  return _ConvertIdToAddress(Id, Offset, 0, rAddress);
}

void MemoryDatabase::ForEachLabel(LabelCallback Callback)
{
  for (auto const& rLblPair : m_LabelsMap)
  {
    Address LblAddr;
    if (!_ConvertIdToAddress(std::get<0>(rLblPair.first), std::get<1>(rLblPair.first), 0, LblAddr))
      // TODO: notify the user
      continue;
    Callback(LblAddr, rLblPair.second);
  }
}

bool MemoryDatabase::AddCrossReference(Address const &rTo, Address const &rFrom)
{
  int IdTo;
  OffsetType OffsetTo;
  if (!_ConvertAddressToId(rTo, IdTo, OffsetTo))
    return false;
  int IdFrom;
  OffsetType OffsetFrom;
  if (!_ConvertAddressToId(rFrom, IdFrom, OffsetFrom))
    return false;
  auto ToPair = std::make_pair(IdTo, OffsetTo);
  auto FromPair = std::make_pair(IdFrom, OffsetFrom);
  m_CrossReferencesToFrom[ToPair].push_back(FromPair);
  m_CrossReferencesFromTo[FromPair].push_back(ToPair);
  return true;
}

bool MemoryDatabase::RemoveCrossReference(Address const &rFrom)
{
  int IdFrom;
  OffsetType OffsetFrom;
  if (!_ConvertAddressToId(rFrom, IdFrom, OffsetFrom))
    return false;
  auto FromPair = std::make_pair(IdFrom, OffsetFrom);
  auto itFromTo = m_CrossReferencesFromTo.find(FromPair);
  if (itFromTo == m_CrossReferencesFromTo.end())
    return false;
  for (auto const& rToPair : itFromTo->second)
  {
    auto itToFrom = m_CrossReferencesToFrom.find(rToPair);
    if (itToFrom != m_CrossReferencesToFrom.end())
    {
      auto& rFromVec = itToFrom->second;
      rFromVec.erase(std::remove(rFromVec.begin(), rFromVec.end(), FromPair), rFromVec.end());
      if (rFromVec.empty())
        m_CrossReferencesToFrom.erase(itToFrom);
    }
  }
  m_CrossReferencesFromTo.erase(itFromTo);
  return true;
}

bool MemoryDatabase::GetCrossReferenceFrom(Address const &rTo, Address::Vector &rFrom) const
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rTo, Id, Offset))
    return false;
  auto it = m_CrossReferencesToFrom.find(std::make_pair(Id, Offset));
  if (it == m_CrossReferencesToFrom.end())
    return false;
  for (auto const& rPair : it->second)
  {
    Address Addr;
    if (_ConvertIdToAddress(rPair.first, rPair.second, 0, Addr))
      rFrom.push_back(Addr);
  }
  return !rFrom.empty();
}

bool MemoryDatabase::GetCrossReferenceTo(Address const &rFrom, Address::Vector &rTo) const
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rFrom, Id, Offset))
    return false;
  auto it = m_CrossReferencesFromTo.find(std::make_pair(Id, Offset));
  if (it == m_CrossReferencesFromTo.end())
    return false;
  for (auto const& rPair : it->second)
  {
    Address Addr;
    if (_ConvertIdToAddress(rPair.first, rPair.second, 0, Addr))
      rTo.push_back(Addr);
  }
  return !rTo.empty();
}

MultiCell::SPType MemoryDatabase::GetMultiCell(Address const &rAddress) const
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto itMultiCellPair = m_MultiCellsMap.find(std::make_pair(Id, Offset));
  if (itMultiCellPair == std::cend(m_MultiCellsMap))
    return false;
  return itMultiCellPair->second;
}

bool MemoryDatabase::SetMultiCell(Address const &rAddress, MultiCell::SPType spMultiCell)
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto const& rMemAreaRel = std::make_pair(Id, Offset);
  auto itMultiCellPair = m_MultiCellsMap.find(rMemAreaRel);
  if (itMultiCellPair != std::cend(m_MultiCellsMap))
    return false;
  m_MultiCellsMap[rMemAreaRel] = spMultiCell;
  return true;
}

bool MemoryDatabase::DeleteMultiCell(Address const &rAddress)
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto itMultiCellPair = m_MultiCellsMap.find(std::make_pair(Id, Offset));
  if (itMultiCellPair == std::cend(m_MultiCellsMap))
    return false;
  m_MultiCellsMap.erase(itMultiCellPair);
  return true;
}

bool MemoryDatabase::GetCellData(Address const &rAddress, CellData &rCellData) const
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  try
  {
    auto const& rCellLayout = m_CellLayoutMap.at(Id);
    if (Offset > rCellLayout.size())
      return false;
    Offset -= rCellLayout[Offset].GetOffset();
    auto itCellDataPair = m_CellsMap.find(std::make_pair(Id, Offset));
    if (itCellDataPair == std::cend(m_CellsMap))
    {
      rCellData = CellData(Cell::ValueType, ValueDetail::HexadecimalType, 1);
      return true;
    }
    rCellData = itCellDataPair->second;
    return true;
  }
  catch (std::exception const&)
  {
  }
  return false;
}

// TODO: handle cells across different memory area
bool MemoryDatabase::SetCellData(Address const &rAddress, CellData const &rCellData, Address::Vector &rDeletedCellAddresses, bool Force)
{
  if (rCellData.GetSize() == 0x0)
    return false;
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto& rCellLayout = m_CellLayoutMap[Id];
  auto CellSize = rCellData.GetSize();
  if (Offset + CellSize > rCellLayout.size())
    return false;
  for (u16 i = 0; i < CellSize; ++i)
    if (!rCellLayout[Offset + i].IsEmpty())
      return false;
  for (u16 i = 0; i < CellSize; ++i)
    rCellLayout[Offset + i].Set(i, CellSize);
  m_CellsMap[std::make_pair(Id, Offset)] = rCellData;
  return true;
}

bool MemoryDatabase::DeleteCellData(Address const &rAddress)
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto& rCellLayout = m_CellLayoutMap[Id];
  if (Offset > rCellLayout.size())
    return false;
  Offset -= rCellLayout[Offset].GetOffset();
  auto CellSize = rCellLayout[Offset].GetSize();
  for (u16 i = 0; i < CellSize; ++i)
    rCellLayout[Offset + i].Clear();
  m_CellsMap.erase(std::make_pair(Id, Offset));
  return true;
}

bool MemoryDatabase::GetComment(Address const &rAddress, std::string &rComment) const
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  auto const itCmt = m_CommentsMap.find(std::make_pair(Id, Offset));
  if (itCmt == std::cend(m_CommentsMap))
    return false;
  rComment = itCmt->second;
  return true;
}

bool MemoryDatabase::SetComment(Address const &rAddress, std::string const &rComment)
{
  int Id;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, Id, Offset))
    return false;
  m_CommentsMap[std::make_pair(Id, Offset)] = rComment;
  return true;
}

bool MemoryDatabase::GetValueDetail(Id ConstId, ValueDetail &rConstDtl) const
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  auto it = m_ValueDetailsMap.find(ConstId);
  if (it == m_ValueDetailsMap.end())
    return false;
  rConstDtl = it->second;
  return true;
}

bool MemoryDatabase::SetValueDetail(Id ConstId, ValueDetail const &rConstDtl)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  m_ValueDetailsMap[ConstId] = rConstDtl;
  return true;
}

bool MemoryDatabase::GetFunctionDetail(Id FuncId, FunctionDetail &rFuncDtl) const
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  auto it = m_FunctionDetailsMap.find(FuncId);
  if (it == m_FunctionDetailsMap.end())
    return false;
  rFuncDtl = it->second;
  return true;
}

bool MemoryDatabase::SetFunctionDetail(Id FuncId, FunctionDetail const &rFuncDtl)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  m_FunctionDetailsMap[FuncId] = rFuncDtl;
  return true;
}

bool MemoryDatabase::GetStructureDetail(Id StructId, StructureDetail &rStructDtl) const
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  auto it = m_StructureDetailsMap.find(StructId);
  if (it == m_StructureDetailsMap.end())
    return false;
  rStructDtl = it->second;
  return true;
}

bool MemoryDatabase::SetStructureDetail(Id StructId, StructureDetail const &rStructDtl)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  m_StructureDetailsMap[StructId] = rStructDtl;
  return true;
}

bool MemoryDatabase::RetrieveDetailId(Address const &rAddress, u8 Index, Id &rDtlId) const
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  int MemId;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, MemId, Offset))
    return false;
  auto Key = std::make_pair(std::make_pair(MemId, Offset), Index);
  auto it = m_DetailBindingsMap.find(Key);
  if (it == m_DetailBindingsMap.end())
    return false;
  rDtlId = it->second;
  return true;
}

bool MemoryDatabase::BindDetailId(Address const &rAddress, u8 Index, Id DtlId)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  int MemId;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, MemId, Offset))
    return false;
  m_DetailBindingsMap[std::make_pair(std::make_pair(MemId, Offset), Index)] = DtlId;
  return true;
}

bool MemoryDatabase::UnbindDetailId(Address const &rAddress, u8 Index)
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  int MemId;
  OffsetType Offset;
  if (!_ConvertAddressToId(rAddress, MemId, Offset))
    return false;
  auto Key = std::make_pair(std::make_pair(MemId, Offset), Index);
  auto it = m_DetailBindingsMap.find(Key);
  if (it == m_DetailBindingsMap.end())
    return false;
  m_DetailBindingsMap.erase(it);
  return true;
}