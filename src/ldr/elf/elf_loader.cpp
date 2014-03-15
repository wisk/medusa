#include "medusa/medusa.hpp"
#include "elf_loader.hpp"

#include <algorithm>

ElfLoader::ElfLoader(void)
{
  memset(m_Ident, 0x0, sizeof(m_Ident));
}

std::string ElfLoader::GetName(void) const
{
  switch (m_Ident[EI_CLASS])
  {
  case ELFCLASSNONE: return "ELF (invalid)";
  case ELFCLASS32:   return "ELF 32-bit";
  case ELFCLASS64:   return "ELF 64-bit";
  default:           return "ELF (unknown)";
  }
}

bool ElfLoader::IsCompatible(BinaryStream const& rBinStrm)
{
  if (rBinStrm.GetSize() < sizeof(Elf32_Ehdr))
    return false;

  if (!rBinStrm.Read(0x0, m_Ident, EI_NIDENT))
    return false;

  if (!rBinStrm.Read(EI_NIDENT + sizeof(u16), m_Machine))
    return false;

  if (memcmp(m_Ident, ELFMAG, SELFMAG))
    return false;

  return true;
}

void ElfLoader::Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs)
{
  switch (m_Ident[EI_CLASS])
  {
  case ELFCLASS32: Map<32>(rDoc); break;
  case ELFCLASS64: Map<64>(rDoc); break;
  default: assert(0 && "Unknown ELF class");
  }
}

void ElfLoader::FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const
{
  std::string ArchName = "";

  switch (m_Machine)
  {
  case EM_386: case EM_X86_64: ArchName = "Intel x86";       break;
  case EM_ARM:                 ArchName = "ARM";             break;
  case EM_AVR:                 ArchName = "Atmel AVR 8-bit"; break;
  default:                                                   break;
  }

  rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [&ArchName](Architecture::SharedPtr spArch)
  { return spArch->GetName() != ArchName;}), std::end(rArchs));
}