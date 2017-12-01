#include "medusa/document.hpp"
#include "medusa/medusa.hpp"
#include "medusa/value.hpp"
#include "medusa/log.hpp"
#include "medusa/module.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

MEDUSA_NAMESPACE_BEGIN

Document::Document(void)
: m_AddressHistoryIndex()
{
}

Document::~Document(void)
{
  Close();
}

bool Document::Open(Database::SPType spDb)
{
  if (m_spDatabase)
    return false;
  m_spDatabase = spDb;
  return true;
}

bool Document::Flush(void)
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->Flush();
}

bool Document::Close(void)
{
  if (m_spDatabase)
  {
    if (!m_spDatabase->Close())
      return false;
    m_spDatabase = nullptr;
  }
  m_QuitSignal();
  std::lock_guard<MutexType> Lock(m_CellMutex);
  m_QuitSignal.disconnect_all_slots();
  m_DocumentUpdatedSignal.disconnect_all_slots();
  m_MemoryAreaUpdatedSignal.disconnect_all_slots();
  m_AddressUpdatedSignal.disconnect_all_slots();
  m_LabelUpdatedSignal.disconnect_all_slots();
  m_TaskUpdatedSignal.disconnect_all_slots();
  return true;
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
    pSubscriber->m_LabelUpdatedConnection = m_LabelUpdatedSignal.connect(boost::bind(&Subscriber::OnLabelUpdated, pSubscriber, _1, _2, _3));

  if (Type & Subscriber::TaskUpdated)
    pSubscriber->m_TaskUpdatedConnection = m_TaskUpdatedSignal.connect(boost::bind(&Subscriber::OnTaskUpdated, pSubscriber, _1, _2));
}

bool Document::GetImageBase(ImageBaseType& rImageBase) const
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->GetImageBase(rImageBase);
}

bool Document::SetImageBase(ImageBaseType ImageBase)
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->SetImageBase(ImageBase);
}

bool Document::GetMemoryArea(Address const& rAddr, MemoryArea& rMemArea) const
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->GetMemoryArea(rAddr, rMemArea);
}

Label Document::GetLabelFromAddress(Address const& rAddr) const
{
  Label CurLbl;
  if (m_spDatabase == nullptr)
    return Label();
  m_spDatabase->GetLabel(rAddr, CurLbl);
  return CurLbl;
}

Address Document::GetAddressFromLabelName(std::string const& rLabelName) const
{
  Address LblAddr;
  if (m_spDatabase == nullptr)
    return Address();
  m_spDatabase->GetLabelAddress(rLabelName, LblAddr);
  return LblAddr;
}

bool Document::AddLabel(Address const& rAddr, Label const& rLabel, bool Force)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  if (rLabel.GetName().empty() && Force)
  {
    RemoveLabel(rAddr);
    return false;
  }
  Label OldLbl, NewLbl = rLabel;
  Address Addr;
  // if (m_spDatabase->GetLabelAddress(NewLbl, Addr))
  // {
  //   do NewLbl.IncrementVersion();
  //   while (m_spDatabase->GetLabelAddress(NewLbl, Addr));
  // }

  if (m_spDatabase->GetLabel(rAddr, OldLbl) == true)
  {
    if (OldLbl.IsAutoGenerated())
      Force = true;

    if (!Force)
      return false;

    if (OldLbl == rLabel)
      return true;

    if (!m_spDatabase->RemoveLabel(rAddr))
      return false;

    m_LabelUpdatedSignal(rAddr, OldLbl, true);
  }
  if (!m_spDatabase->AddLabel(rAddr, NewLbl))
    return false;
  m_LabelUpdatedSignal(rAddr, NewLbl, false);
  m_DocumentUpdatedSignal();
  return true;
}

//bool Document::AddLabels(std::vector<std::pair<Address const&, Label const&>> const & rAddressLabelPairs, bool Force)
//{
//  return false;
//}

bool Document::RemoveLabel(Address const& rAddr)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }

  Label CurLbl;
  if (!m_spDatabase->GetLabel(rAddr, CurLbl))
    return false;
  if (!m_spDatabase->RemoveLabel(rAddr))
    return false;

  m_LabelUpdatedSignal(rAddr, CurLbl, true);
  m_DocumentUpdatedSignal();
  return true;
}

void Document::ForEachLabel(Database::LabelCallback Callback) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return;
  }
  m_spDatabase->ForEachLabel(Callback);
}

bool Document::AddCrossReference(Address const& rTo, Address const& rFrom)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->AddCrossReference(rTo, rFrom);
}

bool Document::RemoveCrossReference(Address const& rFrom)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->RemoveCrossReference(rFrom);
}

bool Document::GetCrossReferenceFrom(Address const& rTo, Address::Vector& rFrom) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->GetCrossReferenceFrom(rTo, rFrom);
}

bool Document::GetCrossReferenceTo(Address const& rFrom, Address::Vector& rTo) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->GetCrossReferenceTo(rFrom, rTo);
}

bool Document::ChangeValueSize(Address const& rValueAddr, u8 NewValueSize, bool Force)
{
  if (NewValueSize == 0x0)
    return false;

  Cell::SPType spOldCell = GetCell(rValueAddr);

  if (spOldCell == nullptr)
    return false;

  if (spOldCell->GetType() != Cell::ValueType && Force == false)
    return false;

  NewValueSize /= 8;

  size_t OldCellLength = spOldCell->GetSize();
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

bool Document::MakeString(Address const& rAddress, u8 StringType, u16 StringLength, bool Force)
{
  OffsetType FileOff;
  if (!ConvertAddressToFileOffset(rAddress, FileOff))
    return false;
  u16 StrLen = GetBinaryStream().StringLength(FileOff);
  if (StrLen == 0)
    return false;
  if (StrLen > StringLength)
    return false;
  ++StrLen; // we want to include '\0'
  auto spNewStr = std::make_shared<String>(StringType, std::min(StrLen, StringLength));
  return SetCell(rAddress, spNewStr, Force);
}

bool Document::GetComment(Address const& rAddress, std::string& rComment) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->GetComment(rAddress, rComment);
}

bool Document::SetComment(Address const& rAddress, std::string const& rComment)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  if (m_spDatabase->SetComment(rAddress, rComment))
  {
    m_DocumentUpdatedSignal();
    return true;
  }
  return false;
}

Cell::SPType Document::GetCell(Address const& rAddr)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return nullptr;
  }
  std::lock_guard<MutexType> Lock(m_CellMutex);

  CellData CurCellData;

  if (!m_spDatabase->GetCellData(rAddr, CurCellData))
    return nullptr;
  auto spCellData = std::make_shared<CellData>(CurCellData); // TODO: we can avoid this

  switch (CurCellData.GetType())
  {
  case Cell::ValueType:     return std::make_shared<Value>(spCellData);
  case Cell::CharacterType: return std::make_shared<Character>(spCellData);
  case Cell::StringType:    return std::make_shared<String>(spCellData);
  case Cell::InstructionType:
    {
      auto spInsn = std::make_shared<Instruction>();
      spInsn->SetArchitectureTag(CurCellData.GetArchitectureTag());
      spInsn->SetMode(CurCellData.GetMode());
      auto spArch = ModuleManager::Instance().GetArchitecture(CurCellData.GetArchitectureTag());
      if (spArch == nullptr)
      {
        Log::Write("core") << "unable to get architecture for " << rAddr << LogEnd;
        return nullptr;
      }
      OffsetType Offset;
      ConvertAddressToFileOffset(rAddr, Offset);
      spArch->Disassemble(GetBinaryStream(), Offset, *spInsn, CurCellData.GetMode());
      return spInsn;
    }
  default:
    break;
  }

  return nullptr;
}

Cell::SPType const Document::GetCell(Address const& rAddr) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return nullptr;
  }
  std::lock_guard<MutexType> Lock(m_CellMutex);

  CellData CurCellData;
  if (!m_spDatabase->GetCellData(rAddr, CurCellData))
    return nullptr;
  auto spCellData = std::make_shared<CellData>(CurCellData); // TODO: we can avoid this

  switch (CurCellData.GetType())
  {
  case Cell::ValueType:     return std::make_shared<Value>(spCellData);
  case Cell::CharacterType: return std::make_shared<Character>(spCellData);
  case Cell::StringType:    return std::make_shared<String>(spCellData);
  case Cell::InstructionType:
    {
      auto spInsn = std::make_shared<Instruction>();
      spInsn->GetData()->SetArchitectureTag(CurCellData.GetArchitectureTag());
      spInsn->SetMode(CurCellData.GetMode());
      auto spArch = ModuleManager::Instance().GetArchitecture(CurCellData.GetArchitectureTag());
      if (spArch == nullptr)
      {
        Log::Write("core") << "unable to get architecture for " << rAddr << LogEnd;
        return nullptr;
      }
      OffsetType Offset;
      ConvertAddressToFileOffset(rAddr, Offset);
      spArch->Disassemble(GetBinaryStream(), Offset, *spInsn, CurCellData.GetMode());
      return spInsn;
    }
  default:
    break;
  }

  return nullptr;
}

u8 Document::GetCellType(Address const& rAddr) const
{
  if (m_spDatabase == nullptr)
  {
    return Cell::CellType;
  }
  CellData CurCellData;
  if (!m_spDatabase->GetCellData(rAddr, CurCellData))
    return Cell::CellType;
  return CurCellData.GetType();
}

u8 Document::GetCellSubType(Address const& rAddr) const
{
  if (m_spDatabase == nullptr)
  {
    return Cell::CellType;
  }
  CellData CurCellData;
  if (!m_spDatabase->GetCellData(rAddr, CurCellData))
    return Cell::CellType;
  return CurCellData.GetSubType();
}

bool Document:: SetArchitecture(Address const& rAddress, Tag TagArch, u8 Mode, Database::SetArchitectureModeType SetArchMode)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->SetArchitecture(rAddress, TagArch, Mode, SetArchMode);
}

bool Document::SetCell(Address const& rAddr, Cell::SPType spCell, bool Force)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  Address::Vector ErasedAddresses;
  ErasedAddresses.push_back(rAddr);
  if (!m_spDatabase->SetCellData(rAddr, *spCell->GetData(), ErasedAddresses, Force))
    return false;
  RemoveLabelIfNeeded(rAddr);

  for (Address const& rErsdAddr : ErasedAddresses)
  {
    Address::Vector Addrs;
    if (GetCrossReferenceFrom(rErsdAddr, Addrs))
      for (auto const& rAddr : Addrs)
        RemoveCrossReference(rAddr);
    if (GetCrossReferenceTo(rErsdAddr, Addrs))
      RemoveCrossReference(rErsdAddr);

    //auto Label = GetLabelFromAddress(rErsdAddr);
    //if (Label.GetType() != Label::Unknown)
    //{
    //  m_LabelUpdatedSignal(rErsdAddr, Label, true);
    //}
  }
  m_AddressUpdatedSignal(ErasedAddresses);

  return true;
}

bool Document::SetCellWithLabel(Address const& rAddr, Cell::SPType spCell, Label const& rLabel, bool Force)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  Address::Vector ErasedAddresses;
  ErasedAddresses.push_back(rAddr);
  if (!m_spDatabase->SetCellData(rAddr, *spCell->GetData(), ErasedAddresses, Force))
    return false;

  RemoveLabelIfNeeded(rAddr);

  for (Address const& rErsdAddr : ErasedAddresses)
    if (GetCell(rErsdAddr) == nullptr)
    {
      RemoveCrossReference(rErsdAddr);

      auto Label = GetLabelFromAddress(rErsdAddr);
      if (Label.GetType() != Label::Unknown)
      {
        m_LabelUpdatedSignal(rErsdAddr, Label, true);
      }
    }

  Label OldLabel;
  if (m_spDatabase->GetLabel(rAddr, OldLabel) == true)
  {
    if (!Force)
      return false;

    if (OldLabel == rLabel)
      return true;

    if (!m_spDatabase->RemoveLabel(rAddr))
      return false;

    m_LabelUpdatedSignal(rAddr, OldLabel, true);
  }
  if (!m_spDatabase->AddLabel(rAddr, rLabel))
    return false;

  m_LabelUpdatedSignal(rAddr, rLabel, false);
  m_AddressUpdatedSignal(ErasedAddresses);

  return true;
}

bool Document::DeleteCell(Address const& rAddr)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  if (!m_spDatabase->DeleteCellData(rAddr))
    return false;

  Address::Vector DelAddr;
  DelAddr.push_back(rAddr);
  m_AddressUpdatedSignal(DelAddr);
  m_DocumentUpdatedSignal();
  RemoveLabelIfNeeded(rAddr);

  return true;
}

MultiCell::SPType Document::GetMultiCell(Address const& rAddr) const
{
  if (m_spDatabase == nullptr)
    return nullptr;
  return m_spDatabase->GetMultiCell(rAddr);
}

bool Document::SetMultiCell(Address const& rAddr, MultiCell::SPType spMultiCell, bool Force)
{
  if (m_spDatabase == nullptr)
    return false;
  if (Force)
    m_spDatabase->DeleteMultiCell(rAddr);
  if (!m_spDatabase->SetMultiCell(rAddr, spMultiCell))
    return false;

  m_DocumentUpdatedSignal();
  Address::Vector Addresses;
  Addresses.push_back(rAddr);
  m_AddressUpdatedSignal(Addresses);

  //if (spMultiCell->GetType() == MultiCell::StructType)
  //{
  //  auto StructId = spMultiCell->GetId();
  //  StructureDetail StructDtl;
  //  if (!GetStructureDetail(StructId, StructDtl))
  //    return true;
  //  if (!_ApplyStructure(rAddr, StructDtl))
  //  {
  //    Log::Write("core") << "failed to apply structure at " << rAddr << LogEnd;
  //  }
  //}

  return true;
}

bool Document::GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->GetValueDetail(ConstId, rConstDtl);
}

bool Document::SetValueDetail(Id ConstId, ValueDetail const& rConstDtl)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->SetValueDetail(ConstId, rConstDtl);
}

bool Document::GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->GetFunctionDetail(FuncId, rFuncDtl);
}

bool Document::SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->SetFunctionDetail(FuncId, rFuncDtl);
}

bool Document::GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->GetStructureDetail(StructId, rStructDtl);
}

bool Document::SetStructureDetail(Id StructId, StructureDetail const& rStructDtl)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->SetStructureDetail(StructId, rStructDtl);
}

bool Document::RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->RetrieveDetailId(rAddress, Index, rDtlId);
}

bool Document::BindDetailId(Address const& rAddress, u8 Index, Id DtlId)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->BindDetailId(rAddress, Index, DtlId);
}

bool Document::UnbindDetailId(Address const& rAddress, u8 Index)
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->UnbindDetailId(rAddress, Index);
}

bool Document::GetDefaultAddressingType(Address::Type& rAddressType) const
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->GetDefaultAddressingType(rAddressType);
}

bool Document::SetDefaultAddressingType(Address::Type AddressType)
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->SetDefaultAddressingType(AddressType);
}

Address Document::MakeAddress(BaseType Base, OffsetType Offset) const
{
  MemoryArea MemArea;
  Address::Type AddrType;
  if (!GetDefaultAddressingType(AddrType))
    return false;
  if (!GetMemoryArea(Address(AddrType, Base, Offset), MemArea))
    return Address();
  Address Addr;
  Addr.SetOffset(Offset);
  return Addr;
}

bool Document::GetPreviousAddressInHistory(Address& rAddress)
{
  std::lock_guard<MutexType> Lock(m_AddressHistoryMutex);

  if (m_AddressHistoryIndex == 0)
    return false;

  --m_AddressHistoryIndex;
  rAddress = m_AddressHistory[m_AddressHistoryIndex];
  return true;
}

bool Document::GetNextAddressInHistory(Address& rAddress)
{
  std::lock_guard<MutexType> Lock(m_AddressHistoryMutex);

  if (m_AddressHistoryIndex + 1 >= m_AddressHistory.size())
    return false;

  rAddress = m_AddressHistory[m_AddressHistoryIndex];
  ++m_AddressHistoryIndex;
  return true;
}

// LATER: it could be better to keep next addresses, but we've to limit the amount of addresses in the container
void Document::InsertAddressInHistory(Address const& rAddress)
{
  std::lock_guard<MutexType> Lock(m_AddressHistoryMutex);

  if (!m_AddressHistory.empty() && m_AddressHistory.back() == rAddress)
    return;

  if (m_AddressHistoryIndex + 1< m_AddressHistory.size())
    m_AddressHistory.erase(std::begin(m_AddressHistory) + m_AddressHistoryIndex + 1, std::end(m_AddressHistory));
  m_AddressHistory.push_back(rAddress);
  if (!m_AddressHistory.empty())
    m_AddressHistoryIndex = m_AddressHistory.size() - 1;
}

bool Document::ConvertAddressToFileOffset(Address const& rAddr, OffsetType& rFileOffset) const
{
  if (m_spDatabase == nullptr)
    return false;

  Address PhysAddr;
  if (!TranslateAddress(rAddr, Address::PhysicalType, PhysAddr))
    return false;
  rFileOffset = PhysAddr.GetOffset();
  return true;
}

bool Document::ConvertAddressToPosition(Address const& rAddr, u32& rPosition) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->ConvertAddressToPosition(rAddr, rPosition);
}

bool Document::ConvertPositionToAddress(u32 Position, Address& rAddr) const
{
  if (m_spDatabase == nullptr)
  {
    Log::Write("core") << "database is null" << LogEnd;
    return false;
  }
  return m_spDatabase->ConvertPositionToAddress(Position, rAddr);
}

bool Document::TranslateAddress(Address const& rAddress, Address::Type ToConvert, Address& rTranslatedAddress) const
{
  if (m_spDatabase == nullptr)
    return false;

  switch (rAddress.GetAddressingType())
  {
  case Address::UnknownType:
    return false;

  case Address::DefaultType:
  {
    Address::Type AddrType;
    if (!m_spDatabase->GetDefaultAddressingType(AddrType))
      return false;
    return TranslateAddress(Address(
      static_cast<Address::Type>(AddrType),
      rAddress.GetBase(), rAddress.GetOffset(),
      rAddress.GetBaseSize(), rAddress.GetOffsetSize()),
      ToConvert, rTranslatedAddress);
  }

  case Address::ArchitectureType:
  {
    if (ToConvert != Address::PhysicalType)
      return false;

    return false; // TODO(wisk): retrieve arch and call translate address method
  }

  default:
    break;
  }

  return m_spDatabase->TranslateAddress(rAddress, ToConvert, rTranslatedAddress);
}

Address Document::GetStartAddress(void) const
{
  if (m_spDatabase == nullptr)
    return Address();
  Address StartAddr;
  if (m_spDatabase->GetLabelAddress(std::string("start"), StartAddr))
    return StartAddr;
  m_spDatabase->GetFirstAddress(StartAddr);
  return StartAddr;
}

Address Document::GetFirstAddress(void) const
{
  if (m_spDatabase == nullptr)
    return Address();
  Address FirstAddr;
  m_spDatabase->GetFirstAddress(FirstAddr);
  return FirstAddr;
}

Address Document::GetLastAddress(void) const
{
  if (m_spDatabase == nullptr)
    return Address();
  Address LastAddr;
  m_spDatabase->GetLastAddress(LastAddr);
  return LastAddr;
}

u32 Document::GetNumberOfAddress(void) const
{
  if (m_spDatabase == nullptr)
    return 0;
  u32 Res = 0;
  m_spDatabase->ForEachMemoryArea([&Res](MemoryArea const& rMemArea)
  {
    Res += static_cast<u32>(rMemArea.GetSize());
  });
  return Res;
}

bool Document::ContainsData(Address const& rAddress) const
{
  return GetCellType(rAddress) != Cell::InstructionType;
}

bool Document::ContainsCode(Address const& rAddress) const
{
  return GetCellType(rAddress) == Cell::InstructionType;
}

bool Document::ContainsUnknown(Address const& rAddress) const
{
  if (m_spDatabase == nullptr)
    return false;
  CellData CurCellData;
  if (!m_spDatabase->GetCellData(rAddress, CurCellData))
    return false;

  return CurCellData.GetType() == Cell::ValueType && CurCellData.GetSize() == 1;
}

Tag Document::GetArchitectureTag(Address const& rAddress) const
{
  Tag ArchTag = MEDUSA_ARCH_UNK;

  if (m_spDatabase == nullptr)
    return false;
  CellData CD;
  if (m_spDatabase->GetCellData(rAddress, CD))
  {
    ArchTag = CD.GetArchitectureTag();
    if (ArchTag != MEDUSA_ARCH_UNK) {
      return ArchTag;
    }
  }
  MemoryArea MemArea;
  if (GetMemoryArea(rAddress, MemArea))
  {
    ArchTag = MemArea.GetArchitectureTag();
    if (ArchTag != MEDUSA_ARCH_UNK)
      return ArchTag;
  }
  return ArchTag;
}

std::list<Tag> Document::GetArchitectureTags(void) const
{
  if (m_spDatabase == nullptr)
    return std::list<Tag>();
  return m_spDatabase->GetArchitectureTags();
}

u8 Document::GetMode(Address const& rAddress) const
{
  u8 Mode = 0;

  auto const spCell = GetCell(rAddress);
  if (spCell != nullptr)
  {
    auto const& rModMgr = ModuleManager::Instance();
    auto ArchTag = spCell->GetArchitectureTag();
    if (ArchTag != MEDUSA_ARCH_UNK)
    {
      auto spCellArch = rModMgr.GetArchitecture(spCell->GetArchitectureTag());
      if (spCellArch != nullptr)
      {
        Mode = spCellArch->GetDefaultMode(rAddress);
        if (Mode != 0)
          return Mode;
      }
      Mode = spCell->GetMode();
      if (Mode != 0)
        return Mode;
    }
  }

  MemoryArea MemArea;
  if (GetMemoryArea(rAddress, MemArea))
  {
    auto spMemAreaArch = ModuleManager::Instance().GetArchitecture(MemArea.GetArchitectureTag());
    if (spMemAreaArch != nullptr)
    {
      Mode = spMemAreaArch->GetDefaultMode(rAddress);
      if (Mode != 0)
        return Mode;
    }
    Mode = MemArea.GetArchitectureMode();
    if (Mode != 0)
      return Mode;
  }

  return Mode;
}

void Document::ForEachMemoryArea(Database::MemoryAreaCallback Callback) const
{
  if (m_spDatabase == nullptr)
    return;
  m_spDatabase->ForEachMemoryArea(Callback);
}

bool Document::AddMemoryArea(MemoryArea const& rMemArea)
{
  if (m_spDatabase == nullptr)
    return false;
  if (!m_spDatabase->AddMemoryArea(rMemArea))
  {
    Log::Write("core") << "unable to add memory area: " << rMemArea.ToString() << LogEnd;
    return false;
  }
  m_MemoryAreaUpdatedSignal(rMemArea, false);
  return true;
}

bool Document::RemoveMemoryArea(MemoryArea const& rMemArea)
{
  if (m_spDatabase == nullptr)
    return false;
  if (!m_spDatabase->RemoveMemoryArea(rMemArea))
  {
    Log::Write("core") << "unable to remove memory area: " << rMemArea.ToString() << LogEnd;
    return false;
  }
  m_MemoryAreaUpdatedSignal(rMemArea, false);
  return true;
}

bool Document::MoveMemoryArea(MemoryArea const& rMemArea, Address const& rBaseAddress)
{
  if (m_spDatabase == nullptr)
    return false;
  if (!m_spDatabase->MoveMemoryArea(rMemArea, rBaseAddress))
  {
    Log::Write("core") << "unable to move memory area: " << rMemArea.ToString() << LogEnd;
    return false;
  }
  m_MemoryAreaUpdatedSignal(rMemArea, false);
  return true;
}

bool Document::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->MoveAddress(rAddress, rMovedAddress, Offset);
}

bool Document::GetPreviousAddress(Address const& rAddress, Address& rPreviousAddress) const
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->MoveAddress(rAddress, rPreviousAddress, -1);
}

bool Document::GetNextAddress(Address const& rAddress, Address& rNextAddress) const
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->MoveAddress(rAddress, rNextAddress, 1);
}

bool Document::GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const
{
  if (m_spDatabase == nullptr)
    return false;
  return m_spDatabase->MoveAddress(rAddress, rNearestAddress, 0);
}

void Document::RemoveLabelIfNeeded(Address const& rAddr)
{
  auto Lbl = GetLabelFromAddress(rAddr);
  if (Lbl.GetType() == Label::Unknown)
    return;
  if (Lbl.GetType() & (Label::Exported | Label::Imported))
    return;

  Address::Vector From;
  if (!GetCrossReferenceFrom(rAddr, From))
    return;
  if (From.size() != 0)
    return;
  RemoveLabel(rAddr);
}

bool Document::_ApplyStructure(Address const& rAddr, StructureDetail const& rStructDtl)
{

  rStructDtl.ForEachField([&](u32 Offset, TypedValueDetail const& rField) -> bool
  {
    Address CurFldAddr = rAddr + Offset;

    std::string Cmt;
    GetComment(CurFldAddr, Cmt);
    Cmt += "struct " + rStructDtl.GetName();
    SetComment(CurFldAddr, Cmt);

    if (!_ApplyTypedValue(rAddr, CurFldAddr, rField))
      return false;

    return true;
  });

  return true;
}

bool Document::_ApplyTypedValue(Address const& rParentAddr, Address const& rAddr, TypedValueDetail const& rTpValDtl)
{
  if ( !_ApplyType(rAddr, rTpValDtl.GetType())
    || !_ApplyValue(rAddr, rTpValDtl.GetValue()))
    return false;

  std::string Cmt;
  GetComment(rAddr, Cmt);
  if (!Cmt.empty())
    Cmt += " ";
  Cmt += rTpValDtl.GetName();

  if (!SetComment(rAddr, Cmt))
    return false;

  auto const& rValDtl = rTpValDtl.GetValue();
  auto RefId = rValDtl.GetRefId();

  switch (rValDtl.GetType())
  {
  case ValueDetail::RelativeType:
  {
    StructureDetail RefStructDtl;
    if (!GetStructureDetail(RefId, RefStructDtl))
      break;
    OffsetType Pos, RefOff;
    if (!ConvertAddressToFileOffset(rAddr, Pos))
      break;
    if (!GetBinaryStream().Read(Pos, RefOff, rTpValDtl.GetSize(), true))
      break;
    if (!_ApplyStructure(rParentAddr + RefOff, RefStructDtl))
      break;

    AddCrossReference(rParentAddr + RefOff, rAddr);

    Log::Write("core") << "relative structure " << RefStructDtl.GetName() << LogEnd;
    break;
  }

  case ValueDetail::CompositeType:
  {
    StructureDetail CpsStructDtl;
    if (!GetStructureDetail(RefId, CpsStructDtl))
      break;
    if (!_ApplyStructure(rAddr, CpsStructDtl))
      break;

    Log::Write("core") << "composite structure " << CpsStructDtl.GetName() << LogEnd;
    break;
  }

  default:
    break;
  }

  return true;
}

bool Document::_ApplyType(Address const& rAddr, TypeDetail::SPType const& rspTpDtl)
{
  return ChangeValueSize(rAddr, rspTpDtl->GetBitSize(), true);
}

bool Document::_ApplyValue(Address const& rAddr, ValueDetail const& rValDtl)
{
  return true;
}

std::string Document::GetOperatingSystemName(void) const
{
  if (m_spDatabase == nullptr)
    return "";
  return m_spDatabase->GetOperatingSystemName();
}

MEDUSA_NAMESPACE_END
