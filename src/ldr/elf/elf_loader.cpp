#include "medusa/medusa.hpp"
#include "elf_loader.hpp"

#include <algorithm>

bool ElfLoader::IsCompatible(BinaryStream const& rBinStrm) const
{
  u8 Ident[EI_NIDENT];
  if (rBinStrm.GetSize() < sizeof(Elf32_Ehdr))
    return false;

  if (!rBinStrm.Read(0x0, Ident, EI_NIDENT))
    return false;

  if (memcmp(Ident, ELFMAG, SELFMAG))
    return false;

  return true;
}

std::string ElfLoader::GetDetailedName(BinaryStream const& rBinStrm) const
{
  u16 Machine;
  std::string MachineName = "<unknown>";
  if (rBinStrm.Read(EI_NIDENT + sizeof(u16), Machine))
  {
    switch (Machine)
    {
    case EM_386:
      MachineName = "x86 (32-bit)"; break;
    case EM_X86_64:
      MachineName = "x86 (64-bit)"; break;
    case EM_ARM:
      MachineName = "Arm"; break;
    case EM_AVR:
      MachineName = "Atmel AVR 8-bit"; break;
    default:
      break;
    }
  }
  return "ELF " + MachineName;
}

std::string ElfLoader::GetSystemName(BinaryStream const& rBinStrm) const
{
  return "unix"; // TODO: be more precise than that
}

std::vector<std::string> ElfLoader::GetUsedArchitectures(BinaryStream const& rBinStrm) const
{
  return { _GetMachineName(rBinStrm) };
}

bool ElfLoader::Map(Document& rDoc) const
{
  switch (_GetBitness(rDoc.GetBinaryStream()))
  {
  case 32: Map<32>(rDoc); break;
  case 64: Map<64>(rDoc); break;
  default: assert(0 && "Unknown ELF class");
  }

  return true;
}

bool ElfLoader::Map(Document& rDoc, Address const& rImgBase) const
{
  // TODO: support loading at a different image base
  return false;
}