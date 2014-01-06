#include "medusa/medusa.hpp"
#include "pe_loader.hpp"

#include <typeinfo>

PeLoader::PeLoader(void) : m_Machine(PE_FILE_MACHINE_UNKNOWN), m_Magic(0x0)
{
}

std::string PeLoader::GetName(void) const
{
  switch (m_Magic)
  {
  case PE_NT_OPTIONAL_HDR32_MAGIC: return "PE";
  case PE_NT_OPTIONAL_HDR64_MAGIC: return "PE+";
  default:                         return "PE (unknown)";
  }
}

bool PeLoader::IsCompatible(BinaryStream const& rBinStrm)
{
  if (rBinStrm.GetSize() < sizeof(PeDosHeader))
    return false;

  PeDosHeader DosHdr;
  if (!rBinStrm.Read(0x0, &DosHdr, sizeof(DosHdr)))
    return false;
  SwapPeDosHeader(DosHdr, LittleEndian);
  if (DosHdr.e_magic != PE_DOS_SIGNATURE)
    return false;

  /* TODO: Sanitize IMAGE_DOS_HEADER::e_lfanew */
  if (rBinStrm.GetSize() < DosHdr.e_lfanew + sizeof(PeNtHeaders32))
    return false;

  u32 Signature;
  PeFileHeader FileHeader;
  if (!rBinStrm.Read(DosHdr.e_lfanew, Signature))
    return false;
  if (Signature != PE_NT_SIGNATURE)
    return false;
  if (!rBinStrm.Read(DosHdr.e_lfanew + sizeof(Signature), &FileHeader, sizeof(FileHeader)))
    return false;
  SwapPeFileHeader(FileHeader, LittleEndian);
  m_Machine = FileHeader.Machine;
  if (!rBinStrm.Read(DosHdr.e_lfanew + sizeof(Signature) + sizeof(PeFileHeader), m_Magic))
    return false;

  return true;
}

void PeLoader::Map(Document& rDoc)
{
  switch (m_Magic)
  {
  case PE_NT_OPTIONAL_HDR32_MAGIC: Map<u32>(rDoc); break;
  case PE_NT_OPTIONAL_HDR64_MAGIC: Map<u64>(rDoc); break;
  default: assert(0 && "Unknown magic");
  }
}

Architecture::SharedPtr PeLoader::GetMainArchitecture(Architecture::VectorSharedPtr const& rArchitectures)
{
  std::string ArchName = "";

  switch (m_Machine)
  {
  case PE_FILE_MACHINE_I386:
  case PE_FILE_MACHINE_AMD64:
    ArchName = "Intel x86";
    break;

  case PE_FILE_MACHINE_ARM:
    ArchName = "ARM";
    break;

  default: break;
  }

  if (ArchName.empty())
    return Architecture::SharedPtr();

  for (auto itArch = std::begin(rArchitectures); itArch != std::end(rArchitectures); ++itArch)
    {
      if ((*itArch)->GetName() == ArchName)
        return *itArch;
    }
  return Architecture::SharedPtr();
}

void PeLoader::Configure(Configuration& rCfg)
{
  switch (m_Magic)
  {
  case PE_NT_OPTIONAL_HDR32_MAGIC: rCfg.Set("Bit", 32); break;
  case PE_NT_OPTIONAL_HDR64_MAGIC: rCfg.Set("Bit", 64); break;
  default: assert(0 && "Unknown magic");
  }
}