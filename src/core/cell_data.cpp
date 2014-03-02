#include <medusa/cell_data.hpp>

#include <sstream>

MEDUSA_NAMESPACE_BEGIN

std::string CellData::Dump(void) const
{
  std::ostringstream oss;
  oss
    << std::hex
    << "dna("   << static_cast<int>(m_Type)
    << " "     << static_cast<int>(m_SubType)
    << " "     << m_Length
    << " "     << m_FormatStyle
    << " "     << static_cast<int>(m_Flags)
    << " "     << static_cast<int>(m_Mode)
    << " "     << m_ArchTag
    << ")";
  return oss.str();
}

MEDUSA_NAMESPACE_END