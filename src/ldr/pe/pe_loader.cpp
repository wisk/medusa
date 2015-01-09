#include "medusa/medusa.hpp"
#include "pe_loader.hpp"

#include <boost/format.hpp>

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
  DosHdr.Swap(LittleEndian);
  if (!DosHdr.IsValid())
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
  FileHeader.Swap(LittleEndian);
  m_Machine = FileHeader.Machine;
  if (!rBinStrm.Read(DosHdr.e_lfanew + sizeof(Signature) + sizeof(PeFileHeader), m_Magic))
    return false;

  return true;
}

void PeLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  switch (m_Magic)
  {
  case PE_NT_OPTIONAL_HDR32_MAGIC: _Map<32>(rDoc, rArchs); break;
  case PE_NT_OPTIONAL_HDR64_MAGIC: _Map<64>(rDoc, rArchs); break;
  default: assert(0 && "Unknown magic");
  }
}

void PeLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
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

  rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [&ArchName](Architecture::SPType spArch)
  { return spArch->GetName() != ArchName; }), std::end(rArchs));
}

bool PeLoader::_FindArchitectureTagAndModeByMachine(
    Architecture::VSPType const& rArchs,
    Tag& rArchTag, u8& rArchMode
    ) const
{
  std::string ArchName = "";
  std::string ArchMode = "";

  switch (m_Machine)
  {
  case PE_FILE_MACHINE_I386:
    ArchName = "Intel x86";
    ArchMode = "32-bit";
    break;

  case PE_FILE_MACHINE_AMD64:
    ArchName = "Intel x86";
    ArchMode = "64-bit";
    break;

  case PE_FILE_MACHINE_ARM:
    ArchName = "ARM";
    break;

  default: break;
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

template<int bit> void PeLoader::_Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  BinaryStream const& rBinStrm = rDoc.GetBinaryStream();

  typedef PeTraits<bit>      PeType;
  typename PeType::DosHeader DosHdr;
  typename PeType::NtHeaders NtHdrs;

  if (!rBinStrm.Read(0x0, &DosHdr, sizeof(DosHdr)))
  {
    Log::Write("ldr_pe") << "unable to read DOS header" << LogEnd;
    return;
  }
  DosHdr.Swap(LittleEndian);

  if (!rBinStrm.Read(DosHdr.e_lfanew, &NtHdrs, sizeof(NtHdrs)))
  {
    Log::Write("ldr_pe") << "unable to read NT headers" << LogEnd;
    return;
  }
  NtHdrs.Swap(LittleEndian);

  u64 ImgBase  = NtHdrs.OptionalHeader.ImageBase;
  u64 EpOff    = ImgBase + NtHdrs.OptionalHeader.AddressOfEntryPoint;
  u16 NumOfScn = NtHdrs.FileHeader.NumberOfSections;
  u64 ScnOff   = DosHdr.e_lfanew + offsetof(typename PeType::NtHeaders, OptionalHeader) + NtHdrs.FileHeader.SizeOfOptionalHeader;

  Log::Write("ldr_pe")
    <<   "ImageBase: "         << ImgBase
    << ", EntryPoint: "        << EpOff
    << ", Number of section: " << NumOfScn
    << LogEnd;

  Address EpAddr(Address::FlatType, 0x0, EpOff, 0x10, bit);
  rDoc.AddLabel(EpAddr, Label("start", Label::Code | Label::Exported));

  _MapSections<bit>(rDoc, rArchs, ImgBase, ScnOff, NumOfScn);
  _ResolveImports<bit>(rDoc, ImgBase,
    NtHdrs.OptionalHeader.DataDirectory[PE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,
    NtHdrs.OptionalHeader.DataDirectory[PE_DIRECTORY_ENTRY_IAT].VirtualAddress);
  _ResolveExports<bit>(rDoc, ImgBase,
    NtHdrs.OptionalHeader.DataDirectory[PE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
}

template<int bit> void PeLoader::_MapSections(Document& rDoc, Architecture::VSPType const& rArchs, u64 ImageBase, u64 SectionHeadersOffset, u16 NumberOfSection)
{
  Tag ArchTag  = MEDUSA_ARCH_UNK;
  u8  ArchMode = 0;

  if (!_FindArchitectureTagAndModeByMachine(rArchs, ArchTag, ArchMode))
    return;

  BinaryStream const&            rBinStrm = rDoc.GetBinaryStream();
  typedef PeTraits<bit>          PeType;
  typename PeType::SectionHeader ScnHdr;

  // TODO: check this
  // ref: https://code.google.com/p/corkami/wiki/PE#SizeOfHeaders
  auto HdrLen = std::min<u32>(0x1000, rBinStrm.GetSize());

  rDoc.AddMemoryArea(new MappedMemoryArea(
    "hdr",
    0x0, HdrLen,
    ImageBase, HdrLen,
    MemoryArea::Read | MemoryArea::Write,
    ArchTag, ArchMode
    ));

  for (u16 ScnIdx = 0; ScnIdx < NumberOfSection; ++ScnIdx)
  {
    u32 Flags = MemoryArea::Read;
    if (!rBinStrm.Read(SectionHeadersOffset + ScnIdx * sizeof(ScnHdr), &ScnHdr, sizeof(ScnHdr)))
    {
      Log::Write("ldr_pe") << "unable to read IMAGE_SECTION_HEADER" << LogEnd;
      continue;
    }
    ScnHdr.Swap(LittleEndian);

    u32 ScnVirtSz = ScnHdr.Misc.VirtualSize ? ScnHdr.Misc.VirtualSize : ScnHdr.SizeOfRawData;
    std::string ScnName(reinterpret_cast<char const*>(ScnHdr.Name), 0, PE_SIZEOF_SHORT_NAME);

    if (ScnHdr.Characteristics & PE_SCN_MEM_EXECUTE)
      Flags |= MemoryArea::Execute;
    // TODO: Is it documented?
    //else if (EpOff >= (ImgBase + ScnHdr.VirtualAddress)
    //  &&     EpOff <   ImgBase + ScnHdr.VirtualAddress + ScnVirtSz)
    //{
    //  Log::Write("ldr_pe") << "promote section " << ScnName << " to executable since it contains the entry point" << LogEnd;
    //  Flags |= MemoryArea::Execute;
    //}
    if (ScnHdr.Characteristics & PE_SCN_MEM_WRITE)
      Flags |= MemoryArea::Write;

    rDoc.AddMemoryArea(new MappedMemoryArea(
      ScnName,
      ScnHdr.PointerToRawData, ScnHdr.SizeOfRawData,
      Address(Address::FlatType, 0x0, ImageBase + ScnHdr.VirtualAddress, 0x10, bit), ScnVirtSz,
      Flags,
      ArchTag, ArchMode
      ));

    Log::Write("ldr_pe") << "found section " << ScnName << LogEnd;
  }
}
template<int bit> void PeLoader::_ResolveImports(Document& rDoc, u64 ImageBase, u64 ImportDirectoryRva, u64 ImportAddressTableRva)
{
  auto const&                       rBinStrm = rDoc.GetBinaryStream();
  typedef PeTraits<bit>             PeType;
  typename PeType::ImportDescriptor CurImp, EndImp;
  ::memset(&EndImp, 0x0, sizeof(EndImp));

  TOffset ImpOff;
  if (!rDoc.ConvertAddressToFileOffset(ImageBase + ImportDirectoryRva, ImpOff))
  {
    Log::Write("ldr_pe") << "unable to convert address import directory" << LogEnd;
    return;
  }

  while (true)
  {
    if (!rBinStrm.Read(ImpOff, &CurImp, sizeof(CurImp)))
    {
      Log::Write("ldr_pe") << "unable to read IMAGE_IMPORT_DESCRIPTOR" << LogEnd;
      return;
    }
    ImpOff += sizeof(CurImp);
    CurImp.Swap(LittleEndian);
    if (!memcmp(&CurImp, &EndImp, sizeof(CurImp)))
      break;

    TOffset ImpNameOff;
    if (!rDoc.ConvertAddressToFileOffset(ImageBase + CurImp.Name, ImpNameOff))
    {
      Log::Write("ldr_pe") << "unable to convert import name address to offset" << LogEnd;
      return;
    }

    std::string ImpName;
    if (!rBinStrm.Read(ImpNameOff, ImpName))
    {
      Log::Write("ldr_pe") << "unable to read import name" << LogEnd;
      return;
    }
    std::transform(std::begin(ImpName), std::end(ImpName), std::begin(ImpName), ::tolower);
    Log::Write("ldr_pe") << "found import: " << ImpName << LogEnd;

    // For each thunks (imported symbol) ...
    std::string       FuncName;
    TOffset           OrgThunkRva = CurImp.OriginalFirstThunk ? CurImp.OriginalFirstThunk : CurImp.FirstThunk,
      ThunkRva = CurImp.FirstThunk,
      FuncOff;
    typename PeType::ThunkData CurOrgThunk, CurThunk, EndThunk = { 0 };

    TOffset OrgThunkOff;
    if (!rDoc.ConvertAddressToFileOffset(ImageBase + OrgThunkRva, OrgThunkOff))
    {
      Log::Write("ldr_pe") << "unable to convert thunk address to offset" << LogEnd;
      return;
    }

    TOffset ThunkOff;
    if (!rDoc.ConvertAddressToFileOffset(ImageBase + ThunkRva, ThunkOff))
    {
      Log::Write("ldr_pe") << "unable to convert original thunk address to offset" << LogEnd;
      return;
    }

    while (true)
    {
      if (!rBinStrm.Read(OrgThunkOff, &CurOrgThunk, sizeof(CurOrgThunk)))
      {
        Log::Write("ldr_pe") << "unable to read original thunk" << LogEnd;
        return;
      }
      OrgThunkOff += sizeof(CurOrgThunk);
      CurOrgThunk.Swap(LittleEndian);
      if (!memcmp(&CurOrgThunk, &EndThunk, sizeof(CurThunk)))
        break;

      if (!rBinStrm.Read(ThunkOff, &CurThunk, sizeof(CurThunk)))
      {
        Log::Write("ldr_pe") << "unable to read thunk" << LogEnd;
        return;
      }
      ThunkOff += sizeof(CurThunk);
      CurThunk.Swap(LittleEndian);
      if (!memcmp(&CurThunk, &EndThunk, sizeof(CurOrgThunk)))
        break;

      std::string SymName = ImpName + "!";

      if (CurThunk.IsOrdinal())
      {
        std::stringstream OrdStrm;
        OrdStrm << CurThunk.GetOrdinal();
        SymName += "ordinal_" + OrdStrm.str();
      }
      else
      {
        // Get ImageImportByName
        if (!rDoc.ConvertAddressToFileOffset(ImageBase + CurOrgThunk.Function, FuncOff))
        {
          Log::Write("ldr_pe") << "unable to convert function address to offset" << LogEnd;
          return;
        }
        std::string ThunkName;
        if (!rBinStrm.Read(FuncOff + offsetof(typename PeType::ImportByName, Name), ThunkName))
        {
          Log::Write("ldr_pe") << "unable to read function name" << LogEnd;
          return;
        }
        SymName += ThunkName;
      }

      Address SymAddr(Address::FlatType, 0x0, ImageBase + ThunkRva, 0, bit);
      ThunkRva += sizeof(typename PeType::ThunkData);
      Log::Write("ldr_pe") << SymAddr << ":   " << SymName << LogEnd;
      rDoc.AddLabel(SymAddr, Label(SymName, Label::Code | Label::Imported));
      rDoc.ChangeValueSize(SymAddr, SymAddr.GetOffsetSize(), true);
      rDoc.BindDetailId(SymAddr, 0, Sha1(SymName));
      auto pFunc = new Function(SymName, 0, 0);
      rDoc.SetMultiCell(SymAddr, pFunc, true);
    }
  }
}

template<int bit> void PeLoader::_ResolveExports(Document& rDoc, u64 ImageBase, u64 ExportDirectoryRva)
{
  auto const& rBinStrm = rDoc.GetBinaryStream();
  typedef PeTraits<bit> PeType;

  typename PeType::ExportDirectory ExpDir;
  TOffset ExpDirOff;
  if (!rDoc.ConvertAddressToFileOffset(ImageBase + ExportDirectoryRva, ExpDirOff))
  {
    Log::Write("ldr_pe") << "unable to convert export directory address to offset" << LogEnd;
    return;
  }
  if (!rBinStrm.Read(ExpDirOff, &ExpDir, sizeof(ExpDir)))
  {
    Log::Write("ldr_pe") << "unable to read export directory" << LogEnd;
    return;
  }

  TOffset FuncOff, NameOff, OrdOff;
  if (!rDoc.ConvertAddressToFileOffset(ImageBase + ExpDir.AddressOfFunctions, FuncOff))
  {
    Log::Write("ldr_pe") << "unable to convert functions address to offset" << LogEnd;
    return;
  }
  if (!rDoc.ConvertAddressToFileOffset(ImageBase + ExpDir.AddressOfNames, NameOff))
  {
    Log::Write("ldr_pe") << "unable to convert names address to offset" << LogEnd;
    return;
  }
  if (!rDoc.ConvertAddressToFileOffset(ImageBase + ExpDir.AddressOfNameOrdinals, OrdOff))
  {
    Log::Write("ldr_pe") << "unable to convert ordinals address to offset" << LogEnd;
    return;
  }

  for (u32 i = 0; i < ExpDir.NumberOfFunctions; ++i)
  {
    u16 Ord;
    if (!rBinStrm.Read(OrdOff + i * sizeof(Ord), Ord))
    {
      Log::Write("ldr_pe") << "unable to read ordinal: " << i << LogEnd;
      continue;
    }

    u32 FuncRva;
    if (!rBinStrm.Read(FuncOff + Ord * sizeof(FuncRva), FuncRva))
    {
      Log::Write("ldr_pe") << "unable to read function rva: " << i << LogEnd;
      continue;
    }

    std::string SymName;
    // Function name if available
    if (i < ExpDir.NumberOfNames)
    {
      u32 SymNameRva;
      if (!rBinStrm.Read(NameOff + i * sizeof(SymNameRva), SymNameRva))
      {
        Log::Write("ldr_pe") << "unable to read export name rva: " << i << LogEnd;
        continue;
      }
      TOffset SymNameOff;
      if (!rDoc.ConvertAddressToFileOffset(ImageBase + SymNameRva, SymNameOff))
      {
        Log::Write("ldr_pe") << "unable to convert export name address to offset" << LogEnd;
        continue;
      }
      if (!rBinStrm.Read(SymNameOff, SymName))
      {
        Log::Write("ldr_pe") << "unable to read export name" << LogEnd;
        continue;
      }
    }
    else
      SymName = (boost::format("ord_%d") % (Ord + ExpDir.Base)).str();

    Address SymAddr(Address::FlatType, 0x0, ImageBase + FuncRva, 0x10, bit);
    rDoc.AddLabel(
      SymAddr,
       // We assume we only export function which is definitely false,
       // but improve the analysis (false positive should be negligible)
      Label(SymName, Label::Exported | Label::Function));
    rDoc.BindDetailId(SymAddr, 0, Sha1(SymName));

    Log::Write("ldr_pe") << "found export name: \"" << SymName << "\", ordinal: " << Ord << LogEnd;
  }
}
