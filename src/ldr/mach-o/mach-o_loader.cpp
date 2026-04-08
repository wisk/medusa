#include "mach-o.h"
#include "mach-o_loader.hpp"
#include "mach-o_traits.hpp"
#include <medusa/medusa.hpp>

#define LOG_WR Log::Write("ldr_mach-o")

#define TEXT_SECTION "__TEXT,__text"

bool MachOLoader::_GetMachineAndEndian(BinaryStream const& rBinStrm, u32& rMachine, bool& rArch64, EEndianness& rEndian, u32& rHeaderOffset)
{
  rHeaderOffset = 0;
  using MachOType = MachOTraits<32>;
  MachOType::MachOHeader  Header;

  if (!rBinStrm.Read(rHeaderOffset, &Header, sizeof(Header))) {
    return false;
  }

  /* TODO : Support Fat binaries */
  if (Header.magic == FAT_MAGIC || Header.magic == FAT_CIGAM) {

    rEndian = (Header.magic == FAT_MAGIC) ? BigEndian : LittleEndian;

    // NumOfFatArch and FatArch structure are always in big endian
    u32 NumOfFatArch = 0;
    if (!rBinStrm.Read(0x4, NumOfFatArch))
      return false;
    EndianSwap(NumOfFatArch);
    Log::Write("ldr_mach-o").Level(LogInfo) << "there's " << NumOfFatArch << " FAT entries" << LogEnd;
    Log::Write("ldr_mach-o").Level(LogWarning) << "universal binary format is not completely supported, picking the first entry..." << LogEnd;

    MachOType::FatArch FatArch;
    if (!rBinStrm.Read(0x8, &FatArch, sizeof(FatArch)))
      return false;
    MachOType::EndianSwap(FatArch, BigEndian);
    rHeaderOffset = FatArch.offset;
    if (!rBinStrm.Read(rHeaderOffset, &Header, sizeof(Header)))
      return false;

  } else if ( Header.magic == MH_MAGIC
    || Header.magic == MH_MAGIC_64) {

      rEndian = LittleEndian;

  } else if (Header.magic == MH_CIGAM
          || Header.magic == MH_CIGAM_64) {

      rEndian = BigEndian;

  } else {
    return false;
  }

  MachOType::EndianSwap(Header, rEndian);

  /* Avoid file types that have not been tested */
  if (Header.filetype != MH_EXECUTE) {
    return false;
  }

  rArch64  = Header.magic == MH_MAGIC_64;
  rMachine = Header.cputype;

  return true;
}

std::string MachOLoader::_GetArchitectureName(u32 Machine, bool Arch64)
{
  switch (Machine)
  {
  case CPU_TYPE_X86:    return "x86/32-bit";
  case CPU_TYPE_X86_64: return "x86/64-bit";
  case CPU_TYPE_ARM:    return "ARM";
  default:              return "";
  }
}

// FIXME(KS): This code is not endian safe
bool MachOLoader::IsCompatible(BinaryStream const& rBinStrm) const
{
  u32 HeaderOffset;
  u32 Machine;
  bool Arch64;
  EEndianness Endian;
  return _GetMachineAndEndian(rBinStrm, Machine, Arch64, Endian, HeaderOffset);
}

std::string MachOLoader::GetName(void) const
{
  return "Mach-O";
}

u8 MachOLoader::GetDepth(void) const
{
  return 1;
}

std::vector<std::string> MachOLoader::GetUsedArchitectures(BinaryStream const& rBinStrm) const
{
  u32 HeaderOffset, Machine;
  bool Arch64;
  EEndianness Endian;
  std::vector<std::string> Result;

  if (_GetMachineAndEndian(rBinStrm, Machine, Arch64, Endian, HeaderOffset))
  {
    auto ArchName = _GetArchitectureName(Machine, Arch64);
    if (!ArchName.empty())
      Result.push_back(ArchName);
  }
  return Result;
}

bool MachOLoader::Map(Document& rDoc) const
{
  BinaryStream const& rBinStrm = rDoc.GetBinaryStream();
  u32 HeaderOffset, Machine;
  bool Arch64;
  EEndianness Endian;

  if (!_GetMachineAndEndian(rBinStrm, Machine, Arch64, Endian, HeaderOffset))
    return false;

  if (Arch64) {
    return MapImpl<64>(rDoc);
  } else {
    return MapImpl<32>(rDoc);
  }
}

bool MachOLoader::Map(Document& rDoc, Address const& rImgBase) const
{
  return false;
}

template<int bit>
bool MachOLoader::MapImpl(Document& rDoc) const
{
  BinaryStream const&             rBinStrm = rDoc.GetBinaryStream();
  auto const&                     rModMngr = ModuleManager::Instance();
  using MachOType = MachOTraits<bit>;
  typename MachOType::MachOHeader Header;
  int                             LoadCmdOff;

  u32 HeaderOffset, Machine;
  bool Arch64;
  EEndianness Endian;
  if (!_GetMachineAndEndian(rBinStrm, Machine, Arch64, Endian, HeaderOffset))
    return false;

  auto ArchName = _GetArchitectureName(Machine, Arch64);
  if (ArchName.empty())
    return false;

  TagType ArchTag;
  u8  ArchMode;
  if (!rModMngr.ConvertArchitectureNameToTagAndMode(ArchName, ArchTag, ArchMode)) {
    LOG_WR << "Cannot identify architecture and mode" << LogEnd;
    return false;
  }

  if (!rBinStrm.Read(HeaderOffset, &Header, sizeof(Header))) {
    LOG_WR << "Cannot read mach-o header" << LogEnd;
    return false;
  }
  MachOType::EndianSwap(Header, Endian);

  u64 EntryPoint = 0;
  EntryPoBitVector EntryPoBV = ENTRYPOINT_NONE;
  u64 TextSectionVMAddr = 0;
  u32 SymbolsOffset = 0;
  u32 StringsOffset = 0;
  u64 ImportsAddress = 0;
  u32 SymbolsIndex = 0;

  LoadCmdOff = HeaderOffset + sizeof(Header);
  for (u32 i = 0; i < Header.ncmds; i++) {
    typename MachOType::LoadCmd LoadCmd;

    if (!rBinStrm.Read(LoadCmdOff, &LoadCmd, sizeof(LoadCmd))) {
      LOG_WR << "Cannot read load command" << LogEnd;
      return false;
    }
    MachOType::EndianSwap(LoadCmd, Endian);

    switch (LoadCmd.cmd) {
    case LC_SEGMENT:
    case LC_SEGMENT_64:
      MapSegment<bit>(rDoc, LoadCmdOff, ArchTag, ArchMode, Endian, HeaderOffset,
                      TextSectionVMAddr, ImportsAddress, SymbolsIndex);
      break;

    case LC_SYMTAB:
      GetSymbols<bit>(rDoc, LoadCmdOff, Endian, SymbolsOffset, StringsOffset);
      break;

    case LC_DYSYMTAB:
      GetDynamicSymbols<bit>(rDoc, LoadCmdOff, Endian, SymbolsOffset, StringsOffset,
                             ImportsAddress, SymbolsIndex);
      break;

      /* Entry point (up to Mac OS X 10.7) */
    case LC_THREAD:
    case LC_UNIXTHREAD:
      GetEntryPointV1(rDoc, LoadCmdOff + sizeof(LoadCmd), Machine, Endian, EntryPoint, EntryPoBV);
      break;

      /* Entry point (since Mac OS X 10.8) */
    case LC_MAIN:
      GetEntryPointV2(rDoc, LoadCmdOff + sizeof(LoadCmd), Endian, EntryPoint, EntryPoBV);
      break;

    default:
      break;
    }

    LoadCmdOff += LoadCmd.cmdsize;
  }

  /* We wait until here to add the start label because we might
  need fields from different load commands. */
  if (EntryPoBV == ENTRYPOINT_OFFSET) {
    if (TextSectionVMAddr) {
      EntryPoint = TextSectionVMAddr + EntryPoint;
    }
  }
  rDoc.AddLabel(Address(Address::LinearType, 0x0, EntryPoint, 0x10, bit),
    Label("start", Label::Code | Label::Exported));

  return true;
}

template<int bit>
void MachOLoader::MapSegment(Document& rDoc, int LoadCmdOff, Tag ArchTag, u8 ArchMode,
                              EEndianness Endian, u32 HeaderOffset,
                              u64& rTextSectionVMAddr, u64& rImportsAddress, u32& rSymbolsIndex) const
{
  BinaryStream const&         rBinStrm = rDoc.GetBinaryStream();
  using MachOType = MachOTraits<bit>;
  typename MachOType::Segment Segment;
  typename MachOType::Section Section;
  std::string                 SegmentName;
  std::string                 FullSectionName;
  auto                        MemAreaFlags = MemoryArea::Access::NoAccess;

  if (!rBinStrm.Read(LoadCmdOff, &Segment, sizeof(Segment))) {
    LOG_WR << "Cannot read segment command" << LogEnd;
    return;
  }
  MachOType::EndianSwap(Segment, Endian);
  SegmentName = reinterpret_cast<char *>(Segment.segname);
  LoadCmdOff += sizeof(Segment);

  for (u32 i = 0; i < Segment.nsects; i++) {
    if (!rBinStrm.Read(LoadCmdOff, &Section, sizeof(Section))) {
      LOG_WR << "Cannot access section in "
        << Segment.segname << " segment"
        << LogEnd;
      return;
    }
    MachOType::EndianSwap(Section, Endian);

    FullSectionName  = SegmentName;
    FullSectionName += ",";
    FullSectionName += reinterpret_cast<char *>(Section.sectname);

    /* We might need this address to compute the entrypoint */
    if (FullSectionName == TEXT_SECTION) {
      rTextSectionVMAddr = Section.addr;
    }

    LOG_WR << "Section found"
      << ": va="     << Section.addr
      << ", offset=" << Section.offset
      << ", size="   << Section.size
      << ", name=\"" << FullSectionName
      << "\""        << LogEnd;

    MemAreaFlags = MemoryArea::Access::Read;
    if (Segment.initprot & VM_PROT_WRITE) {
      MemAreaFlags |= MemoryArea::Access::Write;
    }
    if (Segment.initprot & VM_PROT_EXECUTE) {
      MemAreaFlags |= MemoryArea::Access::Execute;
    }

    bool Res;

    if ((Section.flags & SECTION_TYPE) == S_ZEROFILL) {
      Res = rDoc.AddMemoryArea(MemoryArea::CreateVirtual(
        FullSectionName, MemAreaFlags,
        Address(Address::LinearType, 0x0, Section.addr, 16, bit),
        static_cast<u32>(Section.size),
        ArchTag, ArchMode
        ));
    } else {
      Res = rDoc.AddMemoryArea(MemoryArea::CreateMapped(
        FullSectionName, MemAreaFlags,
        Section.offset,
        static_cast<u32>(Section.size),
        Address(Address::LinearType, 0x0, Section.addr, 16, bit),
        static_cast<u32>(Section.size),
        ArchTag, ArchMode
        ));
    }

    if (!Res)
      LOG_WR << "failed to add memory area" << LogEnd;

    // TODO: Is it the only way to import symbols in Mach-O?
    if ((Section.flags & SECTION_TYPE) == S_LAZY_SYMBOL_POINTERS)
    {
      rImportsAddress = Section.addr;
      rSymbolsIndex   = Section.reserved1;
    }

    LoadCmdOff += sizeof(Section);
  }
}

void MachOLoader::GetEntryPointV1(Document& rDoc, int LoadCmdOff, u32 Machine, EEndianness Endian,
                                   u64& rEntryPoint, EntryPoBitVector& rEntryPoBitVector) const
{
  BinaryStream const& rBinStrm = rDoc.GetBinaryStream();

  switch (Machine) {
  case CPU_TYPE_X86:
    {
      using MachOType = MachOTraits<32>;
      MachOType::x86StateHeader x86StateHeader;
      MachOType::x86State       x86State;

      if (!rBinStrm.Read(LoadCmdOff, &x86StateHeader, sizeof(x86StateHeader))) {
        LOG_WR << "Cannot read processor state header" << LogEnd;
        return;
      }
      MachOType::EndianSwap(x86StateHeader, Endian);
      LoadCmdOff += sizeof(x86StateHeader);

      if (x86StateHeader.flavor != x86_THREAD_STATE32) {
        LOG_WR << "Processor state flavor not supported" << LogEnd;
        return;
      }

      if (!rBinStrm.Read(LoadCmdOff, &x86State, sizeof(x86State))) {
        LOG_WR << "Cannot read processor state" << LogEnd;
        return;
      }
      MachOType::EndianSwap(x86State, Endian);

      rEntryPoint       = x86State.eip;
      rEntryPoBitVector = ENTRYPOINT_VMADDR;
    }
    break;

  case CPU_TYPE_X86_64:
    {
      using MachOType = MachOTraits<64>;
      MachOType::x86StateHeader x86StateHeader;
      MachOType::x86State       x86State;

      if (!rBinStrm.Read(LoadCmdOff, &x86StateHeader, sizeof(x86StateHeader))) {
        LOG_WR << "Cannot read processor state header" << LogEnd;
        return;
      }
      MachOType::EndianSwap(x86StateHeader, Endian);
      LoadCmdOff += sizeof(x86StateHeader);

      if (x86StateHeader.flavor != x86_THREAD_STATE32) {
        LOG_WR << "Processor state flavor not supported" << LogEnd;
        return;
      }

      if (!rBinStrm.Read(LoadCmdOff, &x86State, sizeof(x86State))) {
        LOG_WR << "Cannot read processor state" << LogEnd;
        return;
      }
      MachOType::EndianSwap(x86State, Endian);

      rEntryPoint       = x86State.rip;
      rEntryPoBitVector = ENTRYPOINT_VMADDR;
    }
    break;

  case CPU_TYPE_ARM:
    {
      //TODO
    }
    //break;

  case CPU_TYPE_POWERPC:
    {
      //TODO
    }
    //break;

  default:
    LOG_WR << "Entry point retrieval not implemented" << LogEnd;
    break;
  }
}

void MachOLoader::GetEntryPointV2(Document& rDoc, int LoadCmdOff, EEndianness Endian,
                                   u64& rEntryPoint, EntryPoBitVector& rEntryPoBitVector) const
{
  BinaryStream const&     rBinStrm = rDoc.GetBinaryStream();
  using MachOType = MachOTraits<32>;
  MachOType::EntryPoint   EntryPoint;

  if (!rBinStrm.Read(LoadCmdOff, &EntryPoint, sizeof(EntryPoint))) {
    LOG_WR << "Cannot read entry point command" << LogEnd;
    return;
  }
  MachOType::EndianSwap(EntryPoint, Endian);

  rEntryPoint       = EntryPoint.entryoff;
  rEntryPoBitVector = ENTRYPOINT_OFFSET;
}

template<int bit>
void MachOLoader::GetSymbols(Document& rDoc, int LoadCmdOff, EEndianness Endian,
                              u32& rSymbolsOffset, u32& rStringsOffset) const
{
  BinaryStream const&             rBinStrm = rDoc.GetBinaryStream();
  using MachOType = MachOTraits<bit>;
  typename MachOType::SymbolTable SymTab;

  /* Read Symbol Table */
  if (!rBinStrm.Read(LoadCmdOff, &SymTab, sizeof(SymTab))) {
    LOG_WR << "Cannot read symtab command" << LogEnd;
    return;
  }
  MachOType::EndianSwap(SymTab, Endian);
  rSymbolsOffset = SymTab.symoff;
  rStringsOffset = SymTab.stroff;
}

template<int bit>
void MachOLoader::GetDynamicSymbols(Document& rDoc, int LoadCmdOff, EEndianness Endian,
                                     u32 SymbolsOffset, u32 StringsOffset,
                                     u64 ImportsAddress, u32 SymbolsIndex) const
{
  if (SymbolsOffset == 0x0) {
    LOG_WR << "Unable to get imported symbols, symbol offset is not yet initialized" << LogEnd;
    return;
  }

  BinaryStream const&                    rBinStrm = rDoc.GetBinaryStream();
  using MachOType = MachOTraits<bit>;
  typename MachOType::DynamicSymbolTable DySymTab;

  /* Read Dynamic Symbol Table */
  if (!rBinStrm.Read(LoadCmdOff, &DySymTab, sizeof(DySymTab))) {
    LOG_WR << "Cannot read dysymtab command" << LogEnd;
    return;
  }

  u64 ImpAddr = ImportsAddress;
  for (u32 i = SymbolsIndex; i < DySymTab.nindirectsyms; ++i, ImpAddr += (bit / 8))
  {
    u32 SymIdx;

    if (!rBinStrm.Read(DySymTab.indirectsymoff + sizeof(SymIdx) * i, SymIdx)) {
      LOG_WR << "Cannot read symbol index" << LogEnd;
    }

    typename MachOType::Symbol Sym;

    /* Read Symbol */
    if (!rBinStrm.Read(SymbolsOffset + sizeof(Sym) * SymIdx, &Sym, sizeof(Sym))) {
      LOG_WR << "Cannot read the symbol " << i << LogEnd;
      continue;
    }

    std::string SymName;

    /* Read Symbol Name */
    if (!rBinStrm.Read(StringsOffset + Sym.n_strx, SymName)) {
      LOG_WR << "Cannot read the symbol name " << i << LogEnd;
      continue;
    }

    rDoc.AddLabel(
      Address(Address::LinearType, 0x0, ImpAddr, 0x10, bit),
      Label(SymName, Label::Imported | Label::Data));
  }
}
