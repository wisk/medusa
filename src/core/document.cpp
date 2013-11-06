#include "medusa/document.hpp"
#include "medusa/medusa.hpp"
#include "medusa/value.hpp"
#include "medusa/log.hpp"
#include "medusa/module.hpp"

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

bool Document::Save(Database::SharedPtr spDb)
{
  spDb->SaveBinaryStream(m_rBinaryStream);
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
    if (!spDb->SaveMemoryArea(**itMemArea))
      return false;
  for (auto itMultiCell = std::begin(m_MultiCells); itMultiCell != std::end(m_MultiCells); ++itMultiCell)
    if (!spDb->SaveMultiCell(itMultiCell->first, *itMultiCell->second))
      return false;
  return true;
}

bool Document::Load(Database::SharedPtr spDb)
{
  return false;
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

  if (Type & Subscriber::AddressUpdated)
    pSubscriber->m_AddressUpdatedConnection = m_AddressUpdatedSignal.connect(boost::bind(&Subscriber::OnAddressUpdated, pSubscriber, _1));

  if (Type & Subscriber::LabelUpdated)
    pSubscriber->m_LabelUpdatedConnection = m_LabelUpdatedSignal.connect(boost::bind(&Subscriber::OnLabelUpdated, pSubscriber, _1, _2));

  if (Type & Subscriber::TaskUpdated)
    pSubscriber->m_TaskUpdatedConnection = m_TaskUpdatedSignal.connect(boost::bind(&Subscriber::OnTaskUpdated, pSubscriber, _1, _2));
}

MemoryArea* Document::GetMemoryArea(Address const& rAddr)
{
  boost::lock_guard<MutexType> Lock(m_MemoryAreaMutex);
  for (MemoryAreaSetType::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsCellPresent(rAddr.GetOffset()))
      return *It;

  return nullptr;
}

MemoryArea const* Document::GetMemoryArea(Address const& rAddr) const
{
  boost::lock_guard<MutexType> Lock(m_MemoryAreaMutex);
  for (MemoryAreaSetType::const_iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsCellPresent(rAddr.GetOffset()))
      return *It;

  return nullptr;
}

Label Document::GetLabelFromAddress(Address const& rAddr) const
{
  boost::lock_guard<MutexType> Lock(m_LabelMutex);
  LabelBimapType::left_const_iterator Iter = m_LabelMap.left.find(rAddr);

  if (Iter == m_LabelMap.left.end())
    return Label("", Label::Unknown);

  return Iter->second;
}

void Document::SetLabelToAddress(Address const& rAddr, Label const& rLabel)
{
  boost::lock_guard<MutexType> Lock(m_LabelMutex);
  LabelBimapType::left_iterator Iter = m_LabelMap.left.find(rAddr);
  m_LabelMap.left.replace_data(Iter, rLabel);
  m_LabelUpdatedSignal(rLabel, false);
}

Address Document::GetAddressFromLabelName(std::string const& rLabelName) const
{
  boost::lock_guard<MutexType> Lock(m_LabelMutex);
  LabelBimapType::right_const_iterator Iter = m_LabelMap.right.find(Label(rLabelName, Label::Unknown));

  if (Iter == m_LabelMap.right.end())
    return Address();

  return Iter->second;
}

void Document::AddLabel(Address const& rAddr, Label const& rLabel, bool Force)
{
  auto NewLabel = rLabel;
  auto const& rOldLabel = GetLabelFromAddress(rAddr);
  if (rOldLabel.GetType() != Label::Unknown)
  {
    if (Force == false)
      return;

    RemoveLabel(rAddr);
    if (rOldLabel.GetType() & Label::Exported)
      NewLabel.SetType(NewLabel.GetType() | Label::Exported);
  }

  m_LabelMutex.lock();
  m_LabelMap.left.insert(LabelBimapType::left_value_type(rAddr, NewLabel));
  m_LabelMutex.unlock();
  m_LabelUpdatedSignal(NewLabel, false);
}

void Document::RemoveLabel(Address const& rAddr)
{
  boost::lock_guard<MutexType> Lock(m_LabelMutex);
  auto itLabel = m_LabelMap.left.find(rAddr);
  if (itLabel == std::end(m_LabelMap.left))
    return;
  m_LabelUpdatedSignal(itLabel->second, true);
  m_LabelMap.left.erase(itLabel);
}

bool Document::ChangeValueSize(Address const& rValueAddr, u8 NewValueSize, bool Force)
{
  if (NewValueSize == 0x0)
    return false;

  Cell::SPtr spOldCell = GetCell(rValueAddr);

  if (spOldCell == nullptr)
    return false;

  if (spOldCell->GetType() == Cell::InstructionType && Force == false)
    return false;

  NewValueSize /= 8;

  size_t OldCellLength = spOldCell->GetLength();
  if (spOldCell->GetType() == Cell::ValueType && OldCellLength == NewValueSize)
    return true;

  auto spNewCell = std::make_shared<Value>(spOldCell->GetSubType(), NewValueSize);

  if (NewValueSize > OldCellLength)
    return SetCell(rValueAddr, spNewCell, Force);

  if (SetCell(rValueAddr, spNewCell, Force) == false)
    return false;

  for (u32 i = NewValueSize; i < OldCellLength; ++i)
    if (SetCell(rValueAddr + i, std::make_shared<Value>(), Force) == false)
      return false;

  return true;
}

Cell::SPtr Document::GetCell(Address const& rAddr)
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
    return nullptr;

  m_LastAddressAccessed = rAddr;
  auto spCellData = pMemArea->GetCellData(rAddr.GetOffset());
  if (spCellData == nullptr)
    return Cell::SPtr();

  switch (spCellData->GetType())
  {
  case Cell::ValueType:     return std::make_shared<Value>(spCellData);
  case Cell::CharacterType: return std::make_shared<Character>(spCellData);
  case Cell::StringType:    return std::make_shared<String>(spCellData);
  case Cell::InstructionType:
    {
      auto spInsn = std::make_shared<Instruction>(spCellData);
      auto spArch = ModuleManager::Instance().GetArchitecture(spCellData->GetArchitectureTag());
      TOffset Offset;
      ConvertAddressToFileOffset(rAddr, Offset);
      spInsn->Length() = 0; // reset length to 0
      spArch->Disassemble(m_rBinaryStream, Offset, *spInsn);
      return spInsn;
    }
  default:
    break;
  }

  return Cell::SPtr();
}

Cell::SPtr const Document::GetCell(Address const& rAddr) const
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea const* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
    return nullptr;

  auto spCellData = pMemArea->GetCellData(rAddr.GetOffset());
  if (spCellData == nullptr)
    return Cell::SPtr();

  switch (spCellData->GetType())
  {
  case Cell::ValueType:     return std::make_shared<Value>(spCellData);
  case Cell::CharacterType: return std::make_shared<Character>(spCellData);
  case Cell::StringType:    return std::make_shared<String>(spCellData);
  case Cell::InstructionType:
    {
      auto spInsn = std::make_shared<Instruction>();
      auto spArch = ModuleManager::Instance().GetArchitecture(spCellData->GetArchitectureTag());
      TOffset Offset;
      ConvertAddressToFileOffset(rAddr, Offset);
      spArch->Disassemble(m_rBinaryStream, Offset, *spInsn);
      return spInsn;
    }
  default:
    break;
  }

  return Cell::SPtr();
}

u8 Document::GetCellType(Address const& rAddr) const
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea const* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
    return Cell::CellType;

  auto spCellData = pMemArea->GetCellData(rAddr.GetOffset());
  if (spCellData == nullptr)
    return Cell::CellType;

  return spCellData->GetType();
}

u8 Document::GetCellSubType(Address const& rAddr) const
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea const* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
    return Cell::CellType;

  auto spCellData = pMemArea->GetCellData(rAddr.GetOffset());
  if (spCellData == nullptr)
    return Cell::CellType;

  return spCellData->GetSubType();
}

bool Document::SetCell(Address const& rAddr, Cell::SPtr spCell, bool Force)
{
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
    return false;

  Address::List ErasedAddresses;
  if (pMemArea->SetCellData(rAddr.GetOffset(), spCell->GetData(), ErasedAddresses, Force) == false)
    return false;

  RemoveLabelIfNeeded(rAddr);

  for (auto itAddr = std::begin(ErasedAddresses); itAddr != std::end(ErasedAddresses); ++itAddr)
    if (GetCell(*itAddr) == nullptr)
    {
      if (GetXRefs().HasXRefTo(*itAddr))
        GetXRefs().RemoveRef(*itAddr);

      if (GetXRefs().HasXRefFrom(*itAddr))
      {
        auto Label = GetLabelFromAddress(*itAddr);
        if (Label.GetType() != Label::Unknown)
        {
          m_LabelUpdatedSignal(Label, true);
        }
      }
    }

  Address::List AddressList;
  AddressList.push_back(rAddr);
  AddressList.merge(ErasedAddresses);

  m_DocumentUpdatedSignal();
  m_AddressUpdatedSignal(AddressList);

  m_LastAddressAccessed = rAddr;
  return true;
}

MultiCell* Document::GetMultiCell(Address const& rAddr)
{
  MultiCell::Map::iterator itMultiCell = m_MultiCells.find(rAddr);
  if (itMultiCell == m_MultiCells.end())
    return nullptr;

  return itMultiCell->second;
}

MultiCell const* Document::GetMultiCell(Address const& rAddr) const
{
  MultiCell::Map::const_iterator itMultiCell = m_MultiCells.find(rAddr);
  if (itMultiCell == m_MultiCells.end())
    return nullptr;

  return itMultiCell->second;
}

bool Document::SetMultiCell(Address const& rAddr, MultiCell* pMultiCell, bool Force)
{
  if (Force == false)
  {
    MultiCell::Map::iterator itMultiCell = m_MultiCells.find(rAddr);
    if (itMultiCell != m_MultiCells.end())
      return false;
  }

  m_MultiCells[rAddr] = pMultiCell;

  m_DocumentUpdatedSignal();
  Address::List AddressList;
  AddressList.push_back(rAddr);
  m_AddressUpdatedSignal(AddressList);
  return true;
}

bool Document::ConvertAddressToFileOffset(Address const& rAddr, TOffset& rFileOffset) const
{
  MemoryArea const* pMemoryArea = GetMemoryArea(rAddr);
  if (pMemoryArea == nullptr)
    return false;

  return pMemoryArea->ConvertOffsetToFileOffset(rAddr.GetOffset(), rFileOffset);
}

bool Document::ConvertAddressToPosition(Address const& rAddr, u32& rPosition) const
{
  rPosition = 0;
  boost::mutex::scoped_lock Lock(m_MemoryAreaMutex);
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsCellPresent(rAddr.GetOffset()))
    {
      rPosition += static_cast<u32>(rAddr.GetOffset() - (*itMemArea)->GetBaseAddress().GetOffset());
      return true;
    }
    else
      rPosition += (*itMemArea)->GetSize();
  }
  return false;
}

bool Document::ConvertPositionToAddress(u32 Position, Address& rAddr) const
{
  boost::mutex::scoped_lock Lock(m_MemoryAreaMutex);
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    u32 Size = (*itMemArea)->GetSize();
    if (Position < Size)
    {
      rAddr = (*itMemArea)->GetBaseAddress() + Position;
      return true;
    }
    Position -= Size;
  }
  return false;
}

bool Document::ContainsData(Address const& rAddress) const
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea const* pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return nullptr;

  auto spCellData = pMemArea->GetCellData(rAddress.GetOffset());
  if (spCellData == nullptr)
    return false;

  return spCellData->GetType() != Cell::InstructionType;
}

bool Document::ContainsCode(Address const& rAddress) const
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea const* pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return nullptr;

  auto spCellData = pMemArea->GetCellData(rAddress.GetOffset());
  if (spCellData == nullptr)
    return false;

  return spCellData->GetType() == Cell::InstructionType;
}

bool Document::ContainsUnknown(Address const& rAddress) const
{
  boost::mutex::scoped_lock Lock(m_CellMutex);
  MemoryArea const* pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return nullptr;

  auto spCellData = pMemArea->GetCellData(rAddress.GetOffset());
  if (spCellData == nullptr)
    return false;

  return spCellData->GetType() == Cell::ValueType && spCellData->GetLength() == 1;
}

void Document::AddMemoryArea(MemoryArea* pMemoryArea)
{
  boost::mutex::scoped_lock Lock(m_MemoryAreaMutex);
  m_MemoryAreas.insert(pMemoryArea);
}

bool Document::IsPresent(Address const& Addr) const
{
  boost::mutex::scoped_lock Lock(m_MemoryAreaMutex);
  for (MemoryAreaSetType::const_iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsCellPresent(Addr.GetOffset()))
      return true;

  return false;
}

u32 Document::GetNumberOfAddress(void) const
{
  boost::lock_guard<MutexType> Lock(m_CellMutex);
  u32 Res = 0;
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
    Res += (static_cast<u32>((*itMemArea)->GetSize()));
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
  if (rAddress <= (*m_MemoryAreas.begin())->GetBaseAddress())
  {
    rMovedAddress = rAddress;
    return true;
  }

  auto itMemArea = std::begin(m_MemoryAreas);
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsCellPresent(rAddress.GetOffset()))
      break;
  }
  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  u64 CurMemAreaOff = (rAddress.GetOffset() - (*itMemArea)->GetBaseAddress().GetOffset());
  if (static_cast<u64>(-Offset) <= CurMemAreaOff)
    return (*itMemArea)->MoveAddressBackward(rAddress, rMovedAddress, Offset);
  Offset += CurMemAreaOff;

  if (itMemArea == std::begin(m_MemoryAreas))
    return false;
  --itMemArea;

  bool Failed = false;
  Address CurAddr = ((*itMemArea)->GetBaseAddress() + ((*itMemArea)->GetSize() - 1));
  while (itMemArea != std::begin(m_MemoryAreas))
  {
    u64 MemAreaSize = (*itMemArea)->GetSize();
    if (static_cast<u64>(-Offset) < MemAreaSize)
      break;
    Offset += MemAreaSize;
    CurAddr = ((*itMemArea)->GetBaseAddress() + ((*itMemArea)->GetSize() - 1));

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
    if ((*itMemArea)->IsCellPresent(rAddress.GetOffset()))
      break;
  }
  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  u64 CurMemAreaOff = (rAddress.GetOffset() - (*itMemArea)->GetBaseAddress().GetOffset());
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

  Address CurAddr = (*itMemArea)->GetBaseAddress();
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    u64 MemAreaSize = (*itMemArea)->GetSize();
    if (static_cast<u64>(Offset) < MemAreaSize)
      if ((*itMemArea)->MoveAddressForward(CurAddr, rMovedAddress, Offset) == true)
        return true;
    Offset -= MemAreaSize;
    CurAddr = (*itMemArea)->GetBaseAddress();
  }

  return false;
}

bool Document::GetNextAddress(Address const& rAddress, Address& rNextAddress) const
{
  return MoveAddressForward(rAddress, rNextAddress, 1);
}

bool Document::GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const
{
  auto itMemArea = std::begin(m_MemoryAreas);

  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if (rAddress < (*itMemArea)->GetBaseAddress())
    {
      rNearestAddress = (*itMemArea)->GetBaseAddress();
      return true;
    }

    if ((*itMemArea)->IsCellPresent(rAddress.GetOffset()))
      return (*itMemArea)->GetNearestAddress(rAddress, rNearestAddress);
  }

  return false;
}

void Document::RemoveLabelIfNeeded(Address const& rAddr)
{
  auto Lbl = GetLabelFromAddress(rAddr);
  if (Lbl.GetType() == Label::Unknown)
    return;
  if (Lbl.GetType() & (Label::Exported | Label::Imported))
    return;
  if (!m_XRefs.HasXRefFrom(rAddr))
    RemoveLabel(rAddr);
}

MEDUSA_NAMESPACE_END
