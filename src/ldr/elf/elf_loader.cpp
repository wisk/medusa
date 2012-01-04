#include "medusa/medusa.hpp"
#include "elf_loader.hpp"

#include <typeinfo>

ElfLoader::ElfLoader(Database& rDatabase)
  : Loader(rDatabase)
  , m_rDatabase(rDatabase)
  , m_IsValid(false)
  , m_Machine(EM_NONE)
{
  if (rDatabase.GetFileBinaryStream().GetSize() < sizeof(Elf32_Ehdr))
    return;
  rDatabase.GetFileBinaryStream().Read(0x0, m_Ident, EI_NIDENT);

  if (memcmp(m_Ident, ELFMAG, SELFMAG))
    return;

  switch (GetWordSize())
  {
  case 32: m_Elf._32 = new ElfInterpreter<32>(rDatabase, GetEndianness()); break;
  case 64: m_Elf._64 = new ElfInterpreter<64>(rDatabase, GetEndianness()); break;
  default: return;
  }

  m_IsValid = true;
}

char  const*  ElfLoader::GetName(void)
{
  switch (GetWordSize())
  {
  case 32: return "ELF32";
  case 64: return "ELF64";
  default: return "Invalid ELF";
  }
}


void          ElfLoader::Map(void)
{
  switch (GetWordSize())
  {
  case 32: m_Elf._32->Map(); break;
  case 64: m_Elf._64->Map(); break;
  default: return;
  }
}

void          ElfLoader::Translate(Address const& rVirtAddr, TOffset& rOffset)
{
}

Address ElfLoader::GetEntryPoint(void)
{
  switch (GetWordSize())
  {
  case 32: return m_Elf._32->GetEntryPoint();
  case 64: return m_Elf._64->GetEntryPoint();
  default: return Address();
  }
}

EEndianness          ElfLoader::GetEndianness(void)
{
  switch (m_Ident[EI_DATA])
  {
  case ELFDATA2LSB: return LittleEndian ;
  case ELFDATA2MSB: return BigEndian    ;
  default:          return EndianUnknown;
  }
}

Architecture::Ptr ElfLoader::GetMainArchitecture(Architecture::VectorPtr const& Architectures)
{
  std::string ArchName = "";

  switch (m_Machine)
  {
  case EM_386:
  case EM_X86_64:
    ArchName = "Intel x86";
    break;

  case EM_AVR:
    ArchName = "Atmel AVR 8-bit";
    break;

  default: break;
  }

  if (ArchName.empty())
    return Architecture::Ptr();

  if (Architectures.size() > 0)
    BOOST_FOREACH(Architecture::Ptr pArchitecture, Architectures)
    {
      if (pArchitecture->GetName() == ArchName)
        return pArchitecture;
    }
  return Architecture::Ptr();
}

void ElfLoader::Configure(Configuration& rCfg)
{
  rCfg.Set("Bit", GetWordSize());
}