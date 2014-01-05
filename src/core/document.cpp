#include "medusa/document.hpp"
#include "medusa/medusa.hpp"
#include "medusa/value.hpp"
#include "medusa/log.hpp"
#include "medusa/module.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

MEDUSA_NAMESPACE_BEGIN

Document::~Document(void)
{
  if (m_spDatabase)
    m_spDatabase->Close();
  m_QuitSignal();
  RemoveAll();
}

bool Document::Use(Database::SharedPtr spDb)
{
  if (m_spDatabase)
    return false;
  m_spDatabase = spDb;
  return true;
}

bool Document::Flush(void)
{
  return m_spDatabase->Flush();
}

void Document::RemoveAll(void)
{
  boost::lock_guard<MutexType> Lock(m_MemoryAreaMutex);
  for (MemoryAreaSetType::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    delete *It;
  m_MemoryAreas.erase(m_MemoryAreas.begin(), m_MemoryAreas.end());

  m_MultiCells.erase(m_MultiCells.begin(), m_MultiCells.end());
}

void Document::Connect(u32 Type, Document::Subscriber* pSubscriber)
{
  if (Type & Subscriber::Quit)
    pSubscriber->m_QuitConnection = m_QuitSignal.connect(boost::bind(&Subscriber::OnQuit, pSubscriber));

  if (Type & Subscriber::DocumentUpdated)
    pSubscriber->m_DocumentUpdatedConnection = m_DocumentUpdatedSignal.connect(boost::bind(&Subscriber::OnDocumentUpdated, pSubscriber));

  if (Type & Subscriber::MemoryAreaUpdated)
    pSubscriber->m_MemoryAreaUpdatedConnection = m_MemoryAreaUpdatedSignal.connect(boost::bind(&Subscriber::OnMemoryAreaUpdated, pSubscriber, _1, _2));

  if (Type & Subscriber::AddressUpdated)
    pSubscriber->m_AddressUpdatedConnection = m_AddressUpdatedSignal.connect(boost::bind(&Subscriber::OnAddressUpdated, pSubscriber, _1));

  if (Type & Subscriber::LabelUpdated)
    pSubscriber->m_LabelUpdatedConnection = m_LabelUpdatedSignal.connect(boost::bind(&Subscriber::OnLabelUpdated, pSubscriber, _1, _2));

  if (Type & Subscriber::TaskUpdated)
    pSubscriber->m_TaskUpdatedConnection = m_TaskUpdatedSignal.connect(boost::bind(&Subscriber::OnTaskUpdated, pSubscriber, _1, _2));
}

MemoryArea* Document::GetMemoryArea(Address const& rAddr)
{
  // TODO: add lock
  for (MemoryAreaSetType::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsCellPresent(rAddr))
      return *It;

  return nullptr;
}

MemoryArea const* Document::GetMemoryArea(Address const& rAddr) const
{
  // TODO: add lock
  for (MemoryAreaSetType::const_iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsCellPresent(rAddr))
      return *It;

  return nullptr;
}

Label Document::GetLabelFromAddress(Address const& rAddr) const
{
  Label CurLbl;
  m_spDatabase->GetLabel(rAddr, CurLbl);
  return CurLbl;
}

void Document::SetLabelToAddress(Address const& rAddr, Label const& rLabel)
{
  AddLabel(rAddr, rLabel, true);
}

Address Document::GetAddressFromLabelName(std::string const& rLabelName) const
{
  Address LblAddr;
  m_spDatabase->GetLabelAddress(rLabelName, LblAddr);
  return LblAddr;
}

void Document::AddLabel(Address const& rAddr, Label const& rLabel, bool Force)
{
  Label OldLabel;
  if (m_spDatabase->GetLabel(rAddr, OldLabel) == true)
  {
    if (!Force)
      return;

    if (OldLabel == rLabel)
      return;

    if (!m_spDatabase->RemoveLabel(rAddr))
      return;

    m_LabelUpdatedSignal(OldLabel, true);
  }

  m_spDatabase->AddLabel(rAddr, rLabel);
  m_LabelUpdatedSignal(rLabel, false);
}

void Document::RemoveLabel(Address const& rAddr)
{
  Label CurLbl;
  m_spDatabase->GetLabel(rAddr, CurLbl);
  m_spDatabase->RemoveLabel(rAddr);
  m_LabelUpdatedSignal(CurLbl, true);
}

void Document::ForEachLabel(std::function<void (Address const& rAddress, Label const& rLabel)> LabelPredicat)
{
  m_spDatabase->ForEachLabel(LabelPredicat);
}

bool Document::AddCrossReference(Address const& rTo, Address const& rFrom)
{
  return m_spDatabase->AddCrossReference(rTo, rFrom);
}

bool Document::RemoveCrossReference(Address const& rFrom)
{
  return m_spDatabase->RemoveCrossReference(rFrom);
}

bool Document::RemoveCrossReferences(void)
{
  return m_spDatabase->RemoveCrossReferences();
}

bool Document::HasCrossReferenceFrom(Address const& rTo) const
{
  return m_spDatabase->HasCrossReferenceFrom(rTo);
}

bool Document::GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const
{
  return m_spDatabase->GetCrossReferenceFrom(rTo, rFromList);
}

bool Document::HasCrossReferenceTo(Address const& rFrom) const
{
  return m_spDatabase->HasCrossReferenceTo(rFrom);
}

bool Document::GetCrossReferenceTo(Address const& rFrom, Address& rTo) const
{
  return m_spDatabase->GetCrossReferenceTo(rFrom, rTo);
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
  // TODO: Use database here
  boost::mutex::scoped_lock Lock(m_MemoryAreaMutex);
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
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
      auto spInsn = std::make_shared<Instruction>(spCellData);
      auto spArch = ModuleManager::Instance().GetArchitecture(spCellData->GetArchitectureTag());
      TOffset Offset;
      ConvertAddressToFileOffset(rAddr, Offset);
      spInsn->Length() = 0; // reset length to 0
      spArch->Disassemble(GetBinaryStream(), Offset, *spInsn, spCellData->GetMode());
      return spInsn;
    }
  default:
    break;
  }

  return Cell::SPtr();
}

Cell::SPtr const Document::GetCell(Address const& rAddr) const
{
  // TODO: Use database here
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
      spArch->Disassemble(GetBinaryStream(), Offset, *spInsn, spCellData->GetMode());
      return spInsn;
    }
  default:
    break;
  }

  return Cell::SPtr();
}

u8 Document::GetCellType(Address const& rAddr) const
{
  // TODO: Use database here
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
  // TODO: Use database here
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
  // TODO: Use database here
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == nullptr)
    return false;

  Address::List ErasedAddresses;
  if (pMemArea->SetCellData(rAddr.GetOffset(), spCell->GetData(), ErasedAddresses, Force) == false)
    return false;
  m_spDatabase->SetCellData(rAddr, *spCell->GetData());

  RemoveLabelIfNeeded(rAddr);

  for (auto itAddr = std::begin(ErasedAddresses); itAddr != std::end(ErasedAddresses); ++itAddr)
    if (GetCell(*itAddr) == nullptr)
    {
      if (HasCrossReferenceTo(*itAddr))
        RemoveCrossReference(*itAddr);

      if (HasCrossReferenceFrom(*itAddr))
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

  return true;
}

MultiCell* Document::GetMultiCell(Address const& rAddr)
{
  // TODO: Use database here
  MultiCell::Map::iterator itMultiCell = m_MultiCells.find(rAddr);
  if (itMultiCell == m_MultiCells.end())
    return nullptr;

  return itMultiCell->second;
}

MultiCell const* Document::GetMultiCell(Address const& rAddr) const
{
  // TODO: Use database here
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
  m_spDatabase->AddMultiCell(rAddr, *pMultiCell);

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
    if ((*itMemArea)->IsCellPresent(rAddr))
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
  m_spDatabase->AddMemoryArea(pMemoryArea);
  m_MemoryAreaUpdatedSignal(*pMemoryArea, false);
}

bool Document::IsPresent(Address const& Addr) const
{
  boost::mutex::scoped_lock Lock(m_MemoryAreaMutex);
  for (MemoryAreaSetType::const_iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsCellPresent(Addr))
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
    if ((*itMemArea)->IsCellPresent(rAddress))
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
    if ((*itMemArea)->IsCellPresent(rAddress))
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

    if ((*itMemArea)->IsCellPresent(rAddress))
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
  if (!HasCrossReferenceFrom(rAddr))
    RemoveLabel(rAddr);
}

MEDUSA_NAMESPACE_END
