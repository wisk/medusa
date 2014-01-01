#include "text_db.hpp"

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <sstream>
#include <string>
#include <thread>

std::string wcstr2mbstr(std::wstring const& s)
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
    return false;

  m_TextFile.open(wcstr2mbstr(rDatabasePath), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
  return m_TextFile.is_open();
}

bool TextDatabase::Flush(void)
{
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
  if (HasLabel(rAddress))
    return false;

  std::lock_guard<std::mutex> Lock(m_LabelLock);
  m_LabelMap.left.insert(LabelBimapType::left_value_type(rAddress, rLabel));
  return true;
}

bool TextDatabase::RemoveLabel(Address const& rAddress)
{
  boost::lock_guard<std::mutex> Lock(m_LabelLock);
  auto itLabel = m_LabelMap.left.find(rAddress);
  if (itLabel == std::end(m_LabelMap.left))
    return false;
  m_LabelMap.left.erase(itLabel);
  return true;
}

bool TextDatabase::HasLabel(Address const& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_LabelLock);
  auto itLabel = m_LabelMap.left.find(rAddress);
  return (itLabel != std::end(m_LabelMap.left));
}

bool TextDatabase::GetLabel(Address const& rAddress, Label& rLabel) const
{
  std::lock_guard<std::mutex> Lock(m_LabelLock);
  auto itLabel = m_LabelMap.left.find(rAddress);
  if (itLabel == std::end(m_LabelMap.left))
    return false;
  rLabel = itLabel->second;
  return true;
}

bool TextDatabase::GetLabelAddress(std::string const& rName, Address& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_LabelLock);
  auto itLabel = m_LabelMap.right.find(rName);
  if (itLabel == std::end(m_LabelMap.right))
    return false;

  rAddress = itLabel->second;
  return true;
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
  std::lock_guard<std::mutex> Lock(m_CellsDataLock);
  auto itCellData = m_CellsData.find(rAddress);
  if (itCellData == std::end(m_CellsData))
    return false;
  rCellData = itCellData->second;
  return true;
}

bool TextDatabase::SetCellData(Address const& rAddress, CellData const& rCellData)
{
  std::lock_guard<std::mutex> Lock(m_CellsDataLock);
  m_CellsData[rAddress] = rCellData;
  return true;
}