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

void MemoryArea::CreateUnitializeCell(void)
{
  m_Cells.resize(m_BinStrm.GetSize());

  TOffset CurOff = m_VirtualBase.GetOffset();

  for (TIterator It = Begin(); It != End(); ++It)
  {
    It->first = CurOff++;
    It->second = new Value;
  }
}

Cell* MemoryArea::GetCell(TOffset Off)
{
  if (IsPresent(Off) == false)
    return NULL;
  return m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second;
}

Cell const* MemoryArea::GetCell(TOffset Off) const
{
  if (IsPresent(Off) == false)
    return NULL;
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

  if ((pCell = GetCell(Off)) == NULL)
    return false;

  return SetCell(Off, new Value);
}

bool MemoryArea::EraseCell(TOffset Off)
{
  if (IsPresent(Off) == false)
    return false;
  // NOTE: Since delete(void*) check if the parameter is null or not, we don't have to
  delete m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second;
  m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second = NULL;
  return true;
}

void MemoryArea::Sanitize(TOffset Off, Address::List& rErasedCell)
{
  Cell* pCell = m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second;

  if (pCell == NULL) return;

  // Clean if needed the previous entry
  TOffset PreviousOff = Off;
  Cell* pPreviousCell = NULL;
  if (GetPreviousCell(PreviousOff, pPreviousCell) && pPreviousCell != NULL)
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
  size_t CellMaxLen = pCell->GetLength();
  for (size_t CellLen = 1; CellLen < CellMaxLen; ++CellLen)
  {
    Cell* pCurCell;
    if ((pCurCell = GetCell(Off + CellLen)))
      CellMaxLen += (pCurCell->GetLength() - 1);

    EraseCell(Off + CellLen);
    rErasedCell.push_back(Address(m_VirtualBase.GetAddressingType(), m_VirtualBase.GetBase(), Off + CellLen));

    // If the deleted cell contained data, we fill the gap with Value
    if (CellLen >= pCell->GetLength())
      FillCell(Off + CellLen);
  }

  // XXX: I can't remember why we have to do this...
  //if (m_Cells[0].second == NULL)
  //  m_Cells[0].second = new Value;
}

bool MemoryArea::GetPreviousCell(TOffset& rOff, Cell*& prCell)
{
  prCell = NULL;

  do
  {
    // Avoid integer underflow
    if (rOff == 0x0)
      return false;

    rOff--;

    Cell* pCurrentCell = GetCell(rOff);
    if (pCurrentCell != NULL)
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
  prCell = NULL;

  do
  {
    // Avoid integer overflow (since TOffset is unsigned, -1 means the maximum value)
    if (rOff == -1)
      return false;

    if (LimitSize == 0x0)
      return false;

    rOff++;

    Cell* pCurrentCell = GetCell(rOff);
    if (pCurrentCell != NULL)
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

  if (IsPresent(Off) == false)
    return false;

  // Is there already an allocated cell ?
  if (GetCell(Off) != NULL)
  {
    // If we can't remove it, we return
    if (Force == false)
      return false;

    delete GetCell(Off);
  }
  m_Cells[static_cast<size_t>(Off - m_VirtualBase.GetOffset())].second = pCell;
  rDeletedCell.push_back(
    Address(
    m_VirtualBase.GetAddressingType(),
    m_VirtualBase.GetBase(), Off,
    m_VirtualBase.GetBaseSize(), m_VirtualBase.GetOffsetSize()));

  if (Safe == true)
    Sanitize(Off, rDeletedCell);

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

void MemoryArea::Load(SerializeEntity::SPtr spSrlzEtt)
{
  if (spSrlzEtt->GetName() != "ma")
    throw Exception(L"Database is corrupted (ma is missing)");

  spSrlzEtt->GetField("name",   m_Name    );
  spSrlzEtt->GetField("bs",     m_BinStrm );
  spSrlzEtt->GetField("access", m_Access  );

  for (SerializeEntity::SPtrList::const_iterator It = spSrlzEtt->BeginSubEntities();
    It != spSrlzEtt->EndSubEntities(); ++It)
  {
    TOffset CellOffset;
    Cell::Type CellType;

    (*It)->GetField("type",   CellType  );
    (*It)->GetField("offset", CellOffset);

    // XXX: If we find a cell collision, we ignore the last cell
    if (RetrieveCell(CellOffset) != NULL) continue;

    // Cell factory
    Cell* pCell;
    switch (CellType)
    {
    case Cell::CharacterType:   pCell = new Character;    break;
    case Cell::ValueType:       pCell = new Value;        break;
    case Cell::InstructionType: pCell = new Instruction;  break;
    default:                    pCell = NULL;             break;
    }

    // XXX: We ignore unknown cell
    if (pCell == NULL) continue;

    //pCell->Load(*It);

    // XXX: We should probably notify the UI about these insertions
    Address::List ModifiedAddresses;
    InsertCell(CellOffset, pCell, ModifiedAddresses);
  }
}

SerializeEntity::SPtr MemoryArea::Save(void)
{
  SerializeEntity::SPtr spMemArea(new SerializeEntity("ma"));

  //spMemArea->AddField("name",   m_Name    );
  //spMemArea->AddField("bs",     m_BinStrm );
  //spMemArea->AddField("access", m_Access  );

  //for (TCellMap::const_iterator It = m_Cells.begin();
  //  It != m_Cells.end(); ++It)
  //{
  //  if (It->second == NULL) continue;

  //  //SerializeEntity::SPtr Cell = It->second->Save();
  //  //Cell->AddField("offset", It->first);

  //  if (!Cell) throw Exception(L"Error while saving memory_area");
  //  spMemArea->AddSubEntity(Cell);
  //}

  return spMemArea;
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

void PhysicalMemoryArea::Load(SerializeEntity::SPtr spSrlzEtt)
{
  if (spSrlzEtt->GetName() != "pma") throw Exception(L"Database is corrupted (pma)");

  MemoryArea::Load(spSrlzEtt);
  spSrlzEtt->GetField("pb", m_PhysicalBase);
  spSrlzEtt->GetField("ps", m_PhysicalSize);
}

SerializeEntity::SPtr PhysicalMemoryArea::Save(void)
{
  SerializeEntity::SPtr spPma = MemoryArea::Save();
  spPma->SetName("pma");
  spPma->AddField("pb", m_PhysicalBase);
  spPma->AddField("ps", m_PhysicalSize);
  return spPma;
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

void MappedMemoryArea::Load(SerializeEntity::SPtr spSrlzEtt)
{
  if (spSrlzEtt->GetName() != "mma") throw Exception(L"Database is corrupted (mma)");

  MemoryArea::Load(spSrlzEtt);
  spSrlzEtt->GetField("pb", m_PhysicalBase);
  spSrlzEtt->GetField("ps", m_PhysicalSize);
  spSrlzEtt->GetField("vb", m_VirtualBase);
  spSrlzEtt->GetField("vs", m_VirtualSize);
}

SerializeEntity::SPtr MappedMemoryArea::Save(void)
{
  SerializeEntity::SPtr spMma = MemoryArea::Save();
  spMma->SetName("mma");
  spMma->AddField("pb", m_PhysicalBase);
  spMma->AddField("ps", m_PhysicalSize);
  spMma->AddField("vb", m_VirtualBase);
  spMma->AddField("vs", m_VirtualSize);
  return spMma;
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

void VirtualMemoryArea::Load(SerializeEntity::SPtr spSrlzEtt)
{
  if (spSrlzEtt->GetName() != "vma") throw Exception(L"Database is corrupted (vma)");

  MemoryArea::Load(spSrlzEtt);
  spSrlzEtt->GetField("vb", m_VirtualBase);
  spSrlzEtt->GetField("vs", m_VirtualSize);
}

SerializeEntity::SPtr VirtualMemoryArea::Save(void)
{
  SerializeEntity::SPtr spVma = MemoryArea::Save();
  spVma->SetName("vma");
  spVma->AddField("vb", m_VirtualBase);
  spVma->AddField("vs", m_VirtualSize);
  return spVma;
}

MEDUSA_NAMESPACE_END