#include "medusa/memory_area.hpp"

#include "medusa/value.hpp"
#include "medusa/instruction.hpp"
#include "medusa/character.hpp"

#include <boost/format.hpp>

MEDUSA_NAMESPACE_BEGIN

MemoryArea::MemoryArea(
  std::string const& rName,
  Access Access,
  Tag DefaultArchitectureTag,
  u8 DefaultArchitectureMode)
  : m_Id(0), m_Name(rName)
  , m_Type(UnknownType), m_Access(Access)
  , m_DefaultArchitectureTag(DefaultArchitectureTag), m_DefaultArchitectureMode(DefaultArchitectureMode)
  , m_FileOffset(), m_FileSize()
  , m_BaseAddress(Address::UnknownType, 0xffff, 0x0), m_Size()
{
}

void MemoryArea::SetDefaultArchitectureTag(Tag Arch)
{
  m_DefaultArchitectureTag = Arch;
}

void MemoryArea::SetDefaultArchitectureMode(u8 Mode)
{
  m_DefaultArchitectureMode = Mode;
}

std::string MemoryArea::Dump(void) const
{
  char Buf[4];
  Buf[0] = (m_Access & MemoryArea::Read)    ? 'R' : '-';
  Buf[1] = (m_Access & MemoryArea::Write)   ? 'W' : '-';
  Buf[2] = (m_Access & MemoryArea::Execute) ? 'X' : '-';
  Buf[3] = '\0';

  std::ostringstream oss;
  oss << std::hex << std::showbase;
  oss << "ma(m " << m_Name << " " << m_FileOffset << " " << m_FileSize << " " << m_BaseAddress.Dump() << " " << m_Size << " " << Buf << ")";
  return oss.str();
}

std::string MemoryArea::ToString(void) const
{
  char Buf[4];
  Buf[0] = (m_Access & MemoryArea::Read)    ? 'R' : '-';
  Buf[1] = (m_Access & MemoryArea::Write)   ? 'W' : '-';
  Buf[2] = (m_Access & MemoryArea::Execute) ? 'X' : '-';
  Buf[3] = '\0';
  return (boost::format("; mapped memory area %s %s %#08x %s") % m_Name % m_BaseAddress.ToString() % m_Size % Buf).str();
}

MEDUSA_NAMESPACE_END
