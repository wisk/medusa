#include "medusa/memory_area.hpp"

#include "medusa/value.hpp"
#include "medusa/instruction.hpp"
#include "medusa/character.hpp"

#include <fmt/format.h>

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

std::string MemoryArea::ToString(void) const
{
  char Buf[4];
  Buf[0] = to_bool(m_Access & MemoryArea::Access::Read)    ? 'R' : '-';
  Buf[1] = to_bool(m_Access & MemoryArea::Access::Write)   ? 'W' : '-';
  Buf[2] = to_bool(m_Access & MemoryArea::Access::Execute) ? 'X' : '-';
  Buf[3] = '\0';
  return fmt::format("; mapped memory area {} {} {:#08x} {}", m_Name, m_BaseAddress.ToString(), m_Size, Buf);
}

MEDUSA_NAMESPACE_END
