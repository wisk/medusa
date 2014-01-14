#include "text_db.hpp"

#include <sstream>
#include <string>
#include <thread>

static std::string wcstr2mbstr(std::wstring const& s)
{
  char *mbs = new char[s.length() + 1];
  std::string result;

  if (wcstombs(mbs, s.c_str(), s.length()) == -1)
    throw std::invalid_argument("convertion failed");

  mbs[s.length()] = '\0';

  result = mbs;

  delete[] mbs;

  return result;
}

TextDatabase::TextDatabase(void)
  : m_DelayLabelModification(false)
{
}

TextDatabase::~TextDatabase(void)
{
}

std::string TextDatabase::GetName(void) const
{
  return "Text";
}

std::string TextDatabase::GetExtension(void) const
{
  return ".mdt";
}

bool TextDatabase::IsCompatible(std::wstring const& rDatabasePath) const
{
  std::ifstream File(wcstr2mbstr(rDatabasePath));
  if (File.is_open() == false)
    return false;
  std::string Line;
  std::getline(File, Line);
  return Line == "# Medusa Text Database\n";
}

bool TextDatabase::Open(std::wstring const& rDatabasePath)
{
  m_TextFile.open(wcstr2mbstr(rDatabasePath), std::ios_base::in | std::ios_base::out);
  return m_TextFile.is_open();
}

bool TextDatabase::Create(std::wstring const& rDatabasePath)
{
  // Returns false if we already have a valid file
  if (m_TextFile.is_open())
    return false;

  // Returns false if the file already exists
  m_TextFile.open(wcstr2mbstr(rDatabasePath), std::ios_base::in | std::ios_base::out);
  if (m_TextFile.is_open() == true)
  {
    m_TextFile.close();
    return false;
  }

  m_TextFile.open(wcstr2mbstr(rDatabasePath), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
  return m_TextFile.is_open();
}

bool TextDatabase::Flush(void)
{
  if (!m_TextFile.is_open())
    return false;

  m_TextFile.seekp(0, std::ios::beg);
  m_TextFile.seekg(0, std::ios::beg);

  std::streamoff TotalTell = m_TextFile.tellp() + m_TextFile.tellg();

  if (TotalTell != 0)
    return false;

  m_TextFile << std::hex << std::showbase << "# Medusa Text Database\n";

  // Save binary stream
  {
    m_TextFile << "## BinaryStream\n";
    std::string Base64Data(Base64Type(m_spBinStrm->GetBuffer()), Base64Type(reinterpret_cast<u8 const*>(m_spBinStrm->GetBuffer()) + m_spBinStrm->GetSize()));
    m_TextFile << Base64Data << "\n" << std::flush;
  }

  // Save memory area
  {
    std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
    m_TextFile << "## MemoryArea\n";
    for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
    {
      m_TextFile << (*itMemArea)->Dump() << "\n" << std::flush;
      (*itMemArea)->ForEachCellData([&](TOffset Offset, CellData::SPtr spCellData)
      {
        m_TextFile << "|" << Offset << " " << spCellData->Dump() << "\n" << std::flush;
      });
    }
  }

  // Save label
  {
    std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
    m_TextFile << "## Label\n";
    for (auto itLabel = std::begin(m_LabelMap.left); itLabel != std::end(m_LabelMap.left); ++itLabel)
      m_TextFile << itLabel->first.Dump() << " " << itLabel->second.Dump() << "\n" << std::flush;
  }

  // Save cross reference
  {
    std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
    m_TextFile << "## CrossReference\n";
    for (auto itXref = std::begin(m_CrossReferences.GetAllXRefs().left); itXref != std::end(m_CrossReferences.GetAllXRefs().left); ++itXref)
    {
      m_TextFile << itXref->first.Dump();
      Address::List From;
      m_CrossReferences.From(itXref->first, From);
      for (auto itAddr = std::begin(From); itAddr != std::end(From); ++itAddr)
        m_TextFile << " \xe2\x86\x90" << itAddr->Dump() << std::flush;
      m_TextFile << "\n";
    }
  }

  // Save multicell
  {
    static const char* MultiCellTypeName[] = { "unknown", "function", "string", "structure", "array" };
    std::lock_guard<std::mutex> Lock(m_MultiCellsLock);
    m_TextFile << "## MultiCell\n";
    for (auto itMultiCell = std::begin(m_MultiCells); itMultiCell != std::end(m_MultiCells); ++itMultiCell)
      m_TextFile << itMultiCell->first.Dump() << " " << itMultiCell->second.Dump() << "\n" << std::flush;
  }

  // Save comment
  {
    std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
    m_TextFile << "## Comment\n";
    for (auto itComment = std::begin(m_Comments); itComment != std::end(m_Comments); ++itComment)
    {
      std::string Base64Data(
        Base64Type(itComment->second.data()),
        Base64Type(itComment->second.data() + itComment->second.size()));
      m_TextFile << itComment->first.Dump() << " " << Base64Data << "\n";
    }
  }

  return true;
}

bool TextDatabase::Close(void)
{
  bool Res = Flush();
  m_TextFile.close();
  return Res;
}

bool TextDatabase::AddMemoryArea(MemoryArea* pMemArea)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  m_MemoryAreas.insert(pMemArea);
  return true;
}

MemoryArea const* TextDatabase::GetMemoryArea(Address const& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
    if ((*itMemArea)->IsCellPresent(rAddress))
      return *itMemArea;
  return nullptr;
}

bool TextDatabase::AddLabel(Address const& rAddress, Label const& rLabel)
{
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  if (m_DelayLabelModification)
  {
    m_DelayedLabel.insert(std::make_pair(rAddress, std::make_tuple(rLabel, false)));
    m_DelayedLabelInverse[rLabel.GetName()] = rAddress;
  }
  else
    m_LabelMap.left.insert(LabelBimapType::left_value_type(rAddress, rLabel));
  return true;
}

bool TextDatabase::RemoveLabel(Address const& rAddress)
{
  boost::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  auto itLabel = m_LabelMap.left.find(rAddress);
  if (itLabel == std::end(m_LabelMap.left))
    return false;
  if (m_DelayLabelModification)
  {
    m_DelayedLabel.insert(std::make_pair(itLabel->first, std::make_tuple(itLabel->second, true)));
    m_DelayedLabelInverse[itLabel->second.GetName()] = rAddress;
  }
  else
    m_LabelMap.left.erase(itLabel);
  return true;
}

bool TextDatabase::GetLabel(Address const& rAddress, Label& rLabel) const
{
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  auto itLabel = m_LabelMap.left.find(rAddress);
  if (itLabel == std::end(m_LabelMap.left))
  {
    if (!m_DelayLabelModification)
      return false;

    auto itDelayedLabel = m_DelayedLabel.find(rAddress);
    if (itDelayedLabel != std::end(m_DelayedLabel))
    {
      Address const& rAddr = itDelayedLabel->first;
      Label const& rLbl    = std::get<0>(itDelayedLabel->second);
      bool Remove          = std::get<1>(itDelayedLabel->second);

      if (rAddr == rAddress && Remove == false)
      {
        rLabel = rLbl;
        return true;
      }
    }
    return false;
  }

  rLabel = itLabel->second;
  return true;
}

bool TextDatabase::GetLabelAddress(std::string const& rName, Address& rAddress) const
{
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  auto itLabel = m_LabelMap.right.find(rName);
  if (itLabel == std::end(m_LabelMap.right))
  {
    if (!m_DelayLabelModification)
      return false;

    auto itDelayedLabelInv = m_DelayedLabelInverse.find(rName);
    if (itDelayedLabelInv != std::end(m_DelayedLabelInverse))
    {
      auto itDelayedLabel = m_DelayedLabel.find(itDelayedLabelInv->second);
      Address const& rAddr = itDelayedLabelInv->second;
      Label const& rLbl    = std::get<0>(itDelayedLabel->second);
      bool Remove          = std::get<1>(itDelayedLabel->second);

      if (rLbl.GetName() == rName && Remove == false)
      {
        rAddress = rAddr;
        return true;
      }
    }
    return false;
  }

  rAddress = itLabel->second;
  return true;
}

// This function is not entirely thread-safe
void TextDatabase::ForEachLabel(std::function<void (Address const& rAddress, Label const& rLabel)> LabelPredicat)
{
  m_DelayLabelModification = true;
  for (auto itLabel = std::begin(m_LabelMap.left); itLabel != std::end(m_LabelMap.left); ++itLabel)
  {
    LabelPredicat(itLabel->first, itLabel->second);
  }
  m_DelayLabelModification = false;
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  for (auto itDelayedLabel = std::begin(m_DelayedLabel); itDelayedLabel != std::end(m_DelayedLabel); ++itDelayedLabel)
  {
    Address const& rAddr = itDelayedLabel->first;
    Label const& rLbl    = std::get<0>(itDelayedLabel->second);
    bool Remove          = std::get<1>(itDelayedLabel->second);

    if (Remove)
      RemoveLabel(rAddr);
    else
      AddLabel(rAddr, rLbl);
  }
  m_DelayedLabel.clear();
  m_DelayedLabelInverse.clear();
}

bool TextDatabase::AddCrossReference(Address const& rTo, Address const& rFrom)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.AddXRef(rTo, rFrom);
}

bool TextDatabase::RemoveCrossReference(Address const& rFrom)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.RemoveRef(rFrom);
}

bool TextDatabase::RemoveCrossReferences(void)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  m_CrossReferences.EraseAll();
  return true;
}

bool TextDatabase::HasCrossReferenceFrom(Address const& rTo) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.HasXRefFrom(rTo);
}

bool TextDatabase::GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.From(rTo, rFromList);
}

bool TextDatabase::HasCrossReferenceTo(Address const& rFrom) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.HasXRefTo(rFrom);
}

bool TextDatabase::GetCrossReferenceTo(Address const& rFrom, Address& rTo) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.To(rFrom, rTo);
}

bool TextDatabase::AddMultiCell(Address const& rAddress, MultiCell const& rMultiCell)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  m_MultiCells[rAddress] = rMultiCell;
  return true;
}

bool TextDatabase::RemoveMultiCell(Address const& rAddress)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);

  auto itMultiCell = m_MultiCells.find(rAddress);
  if (itMultiCell == std::end(m_MultiCells))
    return false;
  m_MultiCells.erase(itMultiCell);
  return true;
}

bool TextDatabase::GetMultiCell(Address const& rAddress, MultiCell& rMultiCell) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  auto itMultiCell = m_MultiCells.find(rAddress);
  if (itMultiCell == std::end(m_MultiCells))
    return false;
  rMultiCell = itMultiCell->second;
  return true;
}

bool TextDatabase::GetCellData(Address const& rAddress, CellData& rCellData)
{
  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return false;
  auto spCellData = pMemArea->GetCellData(rAddress.GetOffset());
  if (spCellData == nullptr)
    return false;
  rCellData = *spCellData;
  return true;
}

bool TextDatabase::SetCellData(Address const& rAddress, CellData const& rCellData)
{
  // Text database uses memory area directly to store cell data
  return true;
}

bool TextDatabase::GetComment(Address const& rAddress, std::string& rComment) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  auto itCmt = m_Comments.find(rAddress);
  if (itCmt == std::end(m_Comments))
    return false;
  rComment = itCmt->second;
  return true;
}

bool TextDatabase::SetComment(Address const& rAddress, std::string const& rComment)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  m_Comments[rAddress] = rComment;
  return true;
}