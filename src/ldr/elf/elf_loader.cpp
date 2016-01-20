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

bool ElfLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  switch (m_Ident[EI_CLASS])
  {
  case ELFCLASS32: Map<32>(rDoc, rArchs); break;
  case ELFCLASS64: Map<64>(rDoc, rArchs); break;
  default: assert(0 && "Unknown ELF class");
  }

  return true;
}

bool ElfLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase)
{
  return false;
}

void ElfLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
{
  Tag ArchTag;
  u8  ArchMode;

  if (!FindArchitectureTagAndModeByMachine(rArchs, ArchTag, ArchMode))
    return;

  rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [&ArchTag](Architecture::SPType spArch)
  { return ArchTag != spArch->GetTag();}), std::end(rArchs));
}

bool ElfLoader::FindArchitectureTagAndModeByMachine(
    Architecture::VSPType const& rArchs,
    Tag& rArchTag,
    u8&  rArchMode
    ) const
{
  std::string ArchName = "";
  std::string ArchMode = "";

  switch (m_Machine)
  {
  case EM_386:
    ArchName = "Intel x86";
    ArchMode = "32-bit";
    break;

  case EM_X86_64:
    ArchName = "Intel x86";
    ArchMode = "64-bit";
    break;

  case EM_ARM:                 ArchName = "ARM";             break;
  case EM_AVR:                 ArchName = "Atmel AVR 8-bit"; break;
  default:                                                   break;
  }

  for (auto& rArch : rArchs)
  {
    if (ArchName == rArch->GetName())
    {
      rArchTag  = rArch->GetTag();
      rArchMode = rArch->GetModeByName(ArchMode);
      return true;
    }
  }

  return false;
}
