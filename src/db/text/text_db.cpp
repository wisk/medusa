#include "text_db.hpp"

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <sstream>
#include <string>

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
  std::ifstream File(rDatabasePath);
  if (File.is_open() == false)
    return false;
  std::string Line;
  std::getline(File, Line);
  return Line == "# Medusa Text Database\n";
}

bool TextDatabase::Open(std::wstring const& rDatabasePath)
{
  m_TextFile.open(rDatabasePath, std::ios_base::in | std::ios_base::out);
  return m_TextFile.is_open();
}

bool TextDatabase::Create(std::wstring const& rDatabasePath)
{
  // Returns false if we already have a valid file
  if (m_TextFile.is_open())
    return false;

  // Returns false if the file already exists
  m_TextFile.open(rDatabasePath, std::ios_base::in | std::ios_base::out);
  if (m_TextFile.is_open() == true)
    return false;

  m_TextFile.open(rDatabasePath, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
  return m_TextFile.is_open();
}

bool TextDatabase::Close(void)
{
  m_TextFile.close();
  return true;
}

bool TextDatabase::SaveDocument(Document const& rDoc)
{
  m_TextFile << "# Medusa Text Database" << std::endl;

  typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<u8*, 6, 8>> Base64Type;

  for (auto itMemArea = rDoc.Begin(); itMemArea != rDoc.End(); ++itMemArea)
  {
    u32 MemAreaSize = (*itMemArea)->GetVirtualSize();
    auto pMemAreaData = new u8[MemAreaSize];
    TOffset Off = 0;
    if (!rDoc.Read((*itMemArea)->GetVirtualBase(), pMemAreaData, MemAreaSize))
    {
      delete [] pMemAreaData;
      continue;
    }

    std::string Base64Data(Base64Type(pMemAreaData), Base64Type(pMemAreaData + MemAreaSize));

    m_TextFile << "## Memory Area\n" << (*itMemArea)->ToString() << "\n" << Base64Data << std::endl;
    delete [] pMemAreaData;
  }
  return true;
}

bool TextDatabase::SaveMemoryArea(MemoryArea const& rMemArea)
{
  return true;
}

bool TextDatabase::SaveMultiCell(Address const& rAddress, MultiCell const& rMultiCell)
{
  return true;
}

bool TextDatabase::SaveCell(Address const& rAddress, Cell const& rCell)
{
  return true;
}

bool TextDatabase::SaveLabel(Address const& rAddress, Label const& rLabel)
{
  return true;
}

bool TextDatabase::SaveXRef(Address const& rSrcAddr, Address const& rDstAddr)
{
  return true;
}

bool TextDatabase::LoadDocument(Document& rDoc)
{
  if (m_TextFile.is_open() == false)
    return false;
  return false;
}

bool TextDatabase::LoadMemoryArea(MemoryArea& rMemArea)
{
  return true;
}

bool TextDatabase::LoadMultiCell(Address const& rAddress, MultiCell& rMultiCell)
{
  return true;
}

bool TextDatabase::LoadCell(Address const& rAddress, Cell& rCell)
{
  return true;
}

bool TextDatabase::LoadLabel(Address const& rAddress, Label& rLabel)
{
  return true;
}

bool TextDatabase::LoadXRef(Address& rSrcAddr, Address& rDstAddr)
{
  return true;
}