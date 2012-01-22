#include "medusa/medusa.hpp"
#include "pe_loader.hpp"

#include <typeinfo>

PeLoader::PeLoader(Database& rDatabase)
  : Loader(rDatabase)
  , m_rDatabase(rDatabase)
  , m_IsValid(false)
  , m_Machine(0x0)
{
  if (rDatabase.GetFileBinaryStream().GetSize() < sizeof(PeDosHeader))
    return;

  PeDosHeader DosHdr;
  rDatabase.GetFileBinaryStream().Read(0x0, &DosHdr, sizeof(DosHdr));
  SwapPeDosHeader(DosHdr, LittleEndian);
  if (DosHdr.e_magic != PE_DOS_SIGNATURE)
    return;

  /* TODO: Sanitize IMAGE_DOS_HEADER::e_lfanew */

  if (rDatabase.GetFileBinaryStream().GetSize() < DosHdr.e_lfanew + sizeof(PeNtHeaders32))
    return;

  /*
  Since at this time of interpreting we ignore if we have PE or PE+ file format,
  we assume we have PE for now and then use correct structures later.
  */
  PeNtHeaders32 NtHdrs;
  rDatabase.GetFileBinaryStream().Read(DosHdr.e_lfanew, &NtHdrs, sizeof(NtHdrs));
  SwapPeNtHeaders32(NtHdrs, LittleEndian);

  if (NtHdrs.Signature != PE_NT_SIGNATURE)
    return;

  m_Machine  = NtHdrs.FileHeader.Machine;

  switch (NtHdrs.OptionalHeader.Magic)
  {
  case PE_NT_OPTIONAL_HDR32_MAGIC:
    m_WordSize = 32;
    m_Pe._32   = new PeInterpreter<u32>(rDatabase);
    break;

  case PE_NT_OPTIONAL_HDR64_MAGIC:
    m_WordSize = 64;
    m_Pe._64 = new PeInterpreter<u64>(rDatabase);
    break;

  default: return;
  };

  m_IsValid = true;
}

char  const*  PeLoader::GetName(void)
{
  switch (GetWordSize())
  {
  case 32: return "PE";
  case 64: return "PE+";
  default: return "Invalid PE";
  }
}

void          PeLoader::Map(void)
{
  switch (GetWordSize())
    {
    case 32: m_Pe._32->Map(); break;
    case 64: m_Pe._64->Map(); break;
    default: return;
    }
}

void          PeLoader::Translate(Address const& rVirtAddr, TOffset& rOffset)
{
}

Address PeLoader::GetEntryPoint(void)
{
  switch (GetWordSize())
    {
    case 32: return m_Pe._32->GetEntryPoint();
    case 64: return m_Pe._64->GetEntryPoint();
    default: return Address();
    }
}

Architecture::SPtr PeLoader::GetMainArchitecture(Architecture::VectorSPtr const& Architectures)
{
  std::string ArchName = "";

  switch (m_Machine)
  {
  case PE_FILE_MACHINE_I386:
  case PE_FILE_MACHINE_AMD64:
    ArchName = "Intel x86";
    break;

  default: break;
  }

  if (ArchName.empty())
    return Architecture::SPtr();

  if (Architectures.size() > 0)
    BOOST_FOREACH(Architecture::SPtr pArchitecture, Architectures)
    {
      if (pArchitecture->GetName() == ArchName)
        return pArchitecture;
    }
  return Architecture::SPtr();
}

void PeLoader::Configure(Configuration& rCfg)
{
  rCfg.Set("Bit", GetWordSize());
}