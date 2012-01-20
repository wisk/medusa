#include "medusa/database.hpp"
#include "medusa/medusa.hpp"

#include "medusa/value.hpp"

MEDUSA_NAMESPACE_BEGIN

Database::Database(FileBinaryStream const& rBinaryStream)
  : m_rBinaryStream(rBinaryStream)
  , m_pNotifyCallback(NULL)
{
}

Database::~Database(void)
{
  RemoveAll();
}

MemoryArea* Database::GetMemoryArea(Address const& rAddr)
{
  boost::lock_guard<MutexType> Lock(m_Mutex);
  for (TMemoryAreas::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    if ((*It)->IsPresent(rAddr))
      return *It;

  return NULL;
}

MemoryArea const* Database::GetMemoryArea(Address const& rAddr) const
{
  boost::lock_guard<MutexType> Lock(m_Mutex);
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
}

bool Database::ChangeValueSize(Address const& rValueAddr, u8 NewValueSize, bool Force)
{
  Cell* pOldCell = RetrieveCell(rValueAddr);

  if (pOldCell == NULL)                         return false;
  if (pOldCell->GetType() != Cell::ValueType)   return false;
  size_t OldCellLength = pOldCell->GetLength();
  if (OldCellLength == NewValueSize)            return true;

  u32 ValueType = (static_cast<Value*>(pOldCell)->GetValueType() & VT_MASK);

  switch (NewValueSize)
  {
  case 8:  ValueType |= VS_8BIT;  break;
  case 16: ValueType |= VS_16BIT; break;
  case 32: ValueType |= VS_32BIT; break;
  case 64: ValueType |= VS_64BIT; break;
  default: return false;
  }

  Cell* pNewCell = new Value(ValueType);

  return InsertCell(rValueAddr, pNewCell, Force);
}

Cell* Database::RetrieveCell(Address const& rAddr)
{
  MemoryArea* pMemArea = GetMemoryArea(rAddr);
  if (pMemArea == NULL)
    return NULL;

  return pMemArea->RetrieveCell(rAddr.GetOffset());
}

Cell const* Database::RetrieveCell(Address const& rAddr) const
{
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

  Address::List ModifiedAddresses;
  if (!pMemArea->InsertCell(rAddr.GetOffset(), pCell, ModifiedAddresses, Force, Safe))
    return false;

  if (m_pNotifyCallback)
    m_pNotifyCallback(ModifiedAddresses);

  return true;
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

void Database::RemoveAll(void)
{
  boost::lock_guard<MutexType> Lock(m_Mutex);
  for (TMemoryAreas::iterator It = m_MemoryAreas.begin(); It != m_MemoryAreas.end(); ++It)
    delete *It;
  m_MemoryAreas.erase(m_MemoryAreas.begin(), m_MemoryAreas.end());

  m_MultiCells.erase(m_MultiCells.begin(), m_MultiCells.end());
  m_LabelMap.erase(m_LabelMap.begin(), m_LabelMap.end());
  m_XRefs.EraseAll();
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

void Database::Load(SerializeEntity::SPtr spSrlzEtt)
{
  if (spSrlzEtt->GetName() != "db")
    throw Exception(L"Database is corrupted (db is missing)");

  for (SerializeEntity::SPtrList::const_iterator It = spSrlzEtt->BeginSubEntities();
    It != spSrlzEtt->EndSubEntities(); ++It)
  {
    MemoryArea* pMemArea = NULL;

    if ((*It)->GetName() == "pma")
      pMemArea = new PhysicalMemoryArea;
    else if ((*It)->GetName() == "mma")
      pMemArea = new MappedMemoryArea;
    else if ((*It)->GetName() == "vma")
      pMemArea = new VirtualMemoryArea;
    else
      throw Exception(L"Database is corrupted (unknown memory area type)");

    pMemArea->Load(*It);
    m_MemoryAreas.push_back(pMemArea);
  }
}

SerializeEntity::SPtr Database::Save(void)
{
  SerializeEntity::SPtr spDatabase(new SerializeEntity("db"));

  for (TMemoryAreas::const_iterator It = m_MemoryAreas.begin();
    It != m_MemoryAreas.end(); ++It)
  {
    SerializeEntity::SPtr spMemArea = (*It)->Save();

    if (!spMemArea) throw Exception(L"Error while saving database");
    spDatabase->AddSubEntity(spMemArea);
  }

  return spDatabase;
}

MEDUSA_NAMESPACE_END
