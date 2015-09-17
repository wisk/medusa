#include "mach-o.h"
#include "mach-o_loader.hpp"
#include "mach-o_traits.hpp"
#include <medusa/medusa.hpp>
#include <boost/foreach.hpp>

#define LOG_WR Log::Write("ldr_mach-o")

#define TEXT_SECTION "__TEXT,__text"

MachOLoader::MachOLoader(void) :
  m_HeaderOffset(0),
  m_Machine(0),
  m_Arch64(false),
  m_Endian(EndianUnknown),
  m_EntryPoint(0x0),
  m_EntryPoBitVector(ENTRYPOINT_NONE),
  m_TextSectionVMAddr(0x0),
  m_SymbolsOffset(0x0),
  m_StringsOffset(0x0),
  m_ImportsAddress(0x0),
  m_SymbolsIndex(0x0)
{
}

// FIXME(KS): This code is not endian safe
bool MachOLoader::IsCompatible(BinaryStream const& rBinStrm)
{
  m_HeaderOffset = 0;
  typedef MachOTraits<32> MachOType;
  MachOType::MachOHeader  Header;

  if (!rBinStrm.Read(m_HeaderOffset, &Header, sizeof(Header))) {
    return false;
  }

  /* TODO : Support Fat binaries */
  if (Header.magic == FAT_MAGIC || Header.magic == FAT_CIGAM) {

    m_Endian = (Header.magic == FAT_MAGIC) ? BigEndian : LittleEndian;

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
    m_HeaderOffset = FatArch.offset;
    if (!rBinStrm.Read(m_HeaderOffset, &Header, sizeof(Header)))
      return false;

  } else if ( Header.magic == MH_MAGIC
    || Header.magic == MH_MAGIC_64) {

      m_Endian = LittleEndian;

  } else if (Header.magic == MH_CIGAM
          || Header.magic == MH_CIGAM_64) {

      m_Endian = BigEndian;

  } else {
    return false;
  }

  MachOType::EndianSwap(Header, m_Endian);

  /* Avoid file types that have not been tested */
  if (Header.filetype != MH_EXECUTE) {
    return false;
  }

  m_Arch64  = Header.magic == MH_MAGIC_64;
  m_Machine = Header.cputype;

  return true;
}

std::string MachOLoader::GetName(void) const
{
  return m_Arch64
    ? "Mach-O 64-bit"
    : "Mach-O 32-bit";
}

u8 MachOLoader::GetDepth(void) const
{
  return 1;
}

void MachOLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  if (m_Arch64) {
    Map<64>(rDoc, rArchs);
  } else {
    Map<32>(rDoc, rArchs);
  }
}

template<int bit>
void MachOLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  BinaryStream const&             rBinStrm = rDoc.GetBinaryStream();
  typedef MachOTraits<bit>        MachOType;
  typename MachOType::MachOHeader Header;
  int                             LoadCmdOff;

  Tag ArchTag;
  u8  ArchMode;
  if (!_FindArchitectureTagAndModeByMachine(rArchs, ArchTag, ArchMode)) {
    LOG_WR << "Cannot identify architecture and mode" << LogEnd;
    return;
  }

  if (!rBinStrm.Read(m_HeaderOffset, &Header, sizeof(Header))) {
    LOG_WR << "Cannot read mach-o header" << LogEnd;
    return;
  }
  MachOType::EndianSwap(Header, m_Endian); // TODO(KS): understand which structures are big/little endian...

  LoadCmdOff = m_HeaderOffset + sizeof(Header);
  for (u32 i = 0; i < Header.ncmds; i++) {
    typename MachOType::LoadCmd LoadCmd;

    if (!rBinStrm.Read(LoadCmdOff, &LoadCmd, sizeof(LoadCmd))) {
      LOG_WR << "Cannot read load command" << LogEnd;
      return;
    }
    MachOType::EndianSwap(LoadCmd, m_Endian);

    switch (LoadCmd.cmd) {
    case LC_SEGMENT:
    case LC_SEGMENT_64:
      MachOLoader::MapSegment<bit>(rDoc, LoadCmdOff, ArchTag, ArchMode);
      break;

    case LC_SYMTAB:
      MachOLoader::GetSymbols<bit>(rDoc, LoadCmdOff);
      break;

    case LC_DYSYMTAB:
      MachOLoader::GetDynamicSymbols<bit>(rDoc, LoadCmdOff);
      break;

      /*        case LC_LOAD_DYLIB:
      break;

      case LC_DYSYMTAB:
      break;*/

      /* Entry point (up to Mac OS X 10.7) */
    case LC_THREAD:
    case LC_UNIXTHREAD:
      MachOLoader::GetEntryPointV1(rDoc, LoadCmdOff + sizeof(LoadCmd));
      break;

      /* Entry point (since Mac OS X 10.8) */
    case LC_MAIN:
      MachOLoader::GetEntryPointV2(rDoc, LoadCmdOff + sizeof(LoadCmd));
      break;

    default:
      //printf("[load_command %d]", i);
      //printf("  cmd:%d   size:%d\n",
      //       LoadCmd.cmd & ~LC_REQ_DYLD, LoadCmd.cmdsize);
      break;
    }

    LoadCmdOff += LoadCmd.cmdsize;
  }

  /* We wait until here to add the start label because we might
  need fields from different load commands. */
  if (m_EntryPoBitVector == ENTRYPOINT_OFFSET) {
    if (m_TextSectionVMAddr) {
      m_EntryPoint = m_TextSectionVMAddr + m_EntryPoint;
    }
  }
  rDoc.AddLabel(Address(Address::FlatType, 0x0, m_EntryPoint, 0x10, bit),
    Label("start", Label::Code | Label::Exported));
}

template<int bit>
void MachOLoader::MapSegment(Document& rDoc, int LoadCmdOff, Tag ArchTag, u8 ArchMode)
{
  BinaryStream const&         rBinStrm = rDoc.GetBinaryStream();
  typedef MachOTraits<bit>    MachOType;
  typename MachOType::Segment Segment;
  typename MachOType::Section Section;
  std::string                 SegmentName;
  std::string                 FullSectionName;
  u32                         MemAreaFlags;

  if (!rBinStrm.Read(LoadCmdOff, &Segment, sizeof(Segment))) {
    LOG_WR << "Cannot read segment command" << LogEnd;
    return;
  }
  MachOType::EndianSwap(Segment, m_Endian);
  SegmentName = reinterpret_cast<char *>(Segment.segname);
  LoadCmdOff += sizeof(Segment);

  for (u32 i = 0; i < Segment.nsects; i++) {
    if (!rBinStrm.Read(LoadCmdOff, &Section, sizeof(Section))) {
      LOG_WR << "Cannot access section in "
        << Segment.segname << " segment"
        << LogEnd;
      return;
    }
    MachOType::EndianSwap(Segment, m_Endian);

    FullSectionName  = SegmentName;
    FullSectionName += ",";
    FullSectionName += reinterpret_cast<char *>(Section.sectname);

    /* We might need this address to compute the entrypoint */
    if (FullSectionName == TEXT_SECTION) {
      m_TextSectionVMAddr = Section.addr;
    }

    LOG_WR << "Section found"
      << ": va="     << Section.addr
      << ", offset=" << Section.offset
      << ", size="   << Section.size
      << ", name=\"" << FullSectionName
      << "\""        << LogEnd;

    MemAreaFlags = MemoryArea::Read;
    if (Segment.initprot & VM_PROT_WRITE) {
      MemAreaFlags |= MemoryArea::Write;
    }
    if (Segment.initprot & VM_PROT_EXECUTE) {
      MemAreaFlags |= MemoryArea::Execute;
    }

    MemoryArea* pNewMemArea = nullptr;

    if ((Section.flags & SECTION_TYPE) == S_ZEROFILL) {
      pNewMemArea = new VirtualMemoryArea(
        FullSectionName,
        Address(Address::FlatType, 0x0, Section.addr, 16, bit),
        static_cast<u32>(Section.size),
        MemAreaFlags,
        ArchTag, ArchMode
        );
    } else {
      pNewMemArea = new MappedMemoryArea(
        FullSectionName,
        Section.offset,
        static_cast<u32>(Section.size),
        Address(Address::FlatType, 0x0, Section.addr, 16, bit),
        static_cast<u32>(Section.size),
        MemAreaFlags,
        ArchTag, ArchMode
        );
    }

    rDoc.AddMemoryArea(pNewMemArea);

    if ((Section.flags & SECTION_TYPE) == S_CSTRING_LITERALS)
      for (Address CurAddr = pNewMemArea->GetBaseAddress(), EndAddr = pNewMemArea->GetBaseAddress() + pNewMemArea->GetSize();
        CurAddr < EndAddr;)
      {
        if (!rDoc.MakeString(CurAddr, String::Utf8Type))
        {
          CurAddr += 1;
          continue;
        }
        if (!rDoc.GetNextAddress(CurAddr, CurAddr))
        {
          CurAddr += 1;
          continue;
        }
      }

    // TODO: Is it the only way to import symbols in Mach-O?
    if ((Section.flags & SECTION_TYPE) == S_LAZY_SYMBOL_POINTERS)
    {
      m_ImportsAddress = Section.addr;
      m_SymbolsIndex   = Section.reserved1;
    }

      LoadCmdOff += sizeof(Section);
  }
}

void MachOLoader::GetEntryPointV1(Document& rDoc, int LoadCmdOff)
{
  BinaryStream const& rBinStrm = rDoc.GetBinaryStream();

  switch (m_Machine) {
  case CPU_TYPE_X86:
    {
      typedef MachOTraits<32>   MachOType;
      MachOType::x86StateHeader x86StateHeader;
      MachOType::x86State       x86State;

      if (!rBinStrm.Read(LoadCmdOff, &x86StateHeader, sizeof(x86StateHeader))) {
        LOG_WR << "Cannot read processor state header" << LogEnd;
        return;
      }
      MachOType::EndianSwap(x86StateHeader, m_Endian);
      LoadCmdOff += sizeof(x86StateHeader);

      if (x86StateHeader.flavor != x86_THREAD_STATE32) {
        LOG_WR << "Processor state flavor not supported" << LogEnd;
        return;
      }

      if (!rBinStrm.Read(LoadCmdOff, &x86State, sizeof(x86State))) {
        LOG_WR << "Cannot read processor state" << LogEnd;
        return;
      }
      MachOType::EndianSwap(x86State, m_Endian);

      m_EntryPoint     = x86State.eip;
      m_EntryPoBitVector = ENTRYPOINT_VMADDR;
    }
    break;

  case CPU_TYPE_X86_64:
    {
      typedef MachOTraits<64>   MachOType;
      MachOType::x86StateHeader x86StateHeader;
      MachOType::x86State       x86State;

      if (!rBinStrm.Read(LoadCmdOff, &x86StateHeader, sizeof(x86StateHeader))) {
        LOG_WR << "Cannot read processor state header" << LogEnd;
        return;
      }
      MachOType::EndianSwap(x86StateHeader, m_Endian);
      LoadCmdOff += sizeof(x86StateHeader);

      if (x86StateHeader.flavor != x86_THREAD_STATE32) {
        LOG_WR << "Processor state flavor not supported" << LogEnd;
        return;
      }

      if (!rBinStrm.Read(LoadCmdOff, &x86State, sizeof(x86State))) {
        LOG_WR << "Cannot read processor state" << LogEnd;
        return;
      }
      MachOType::EndianSwap(x86State, m_Endian);

      m_EntryPoint     = x86State.rip;
      m_EntryPoBitVector = ENTRYPOINT_VMADDR;
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

void MachOLoader::GetEntryPointV2(Document& rDoc, int LoadCmdOff)
{
  BinaryStream const&     rBinStrm = rDoc.GetBinaryStream();
  typedef MachOTraits<32> MachOType;
  MachOType::EntryPoint   EntryPoint;

  if (!rBinStrm.Read(LoadCmdOff, &EntryPoint, sizeof(EntryPoint))) {
    LOG_WR << "Cannot read entry point command" << LogEnd;
    return;
  }
  MachOType::EndianSwap(EntryPoint, m_Endian);

  m_EntryPoint     = EntryPoint.entryoff;
  m_EntryPoBitVector = ENTRYPOINT_OFFSET;
}

template<int bit>
void MachOLoader::GetSymbols(Document& rDoc, int LoadCmdOff)
{
  BinaryStream const&             rBinStrm = rDoc.GetBinaryStream();
  typedef MachOTraits<bit>        MachOType;
  typename MachOType::SymbolTable SymTab;

  /* Read Symbol Table */
  if (!rBinStrm.Read(LoadCmdOff, &SymTab, sizeof(SymTab))) {
    LOG_WR << "Cannot read symtab command" << LogEnd;
    return;
  }
  MachOType::EndianSwap(SymTab, m_Endian);
  m_SymbolsOffset = SymTab.symoff;
  m_StringsOffset = SymTab.stroff;
}

template<int bit>
void MachOLoader::GetDynamicSymbols(Document& rDoc, int LoadCmdOff)
{
  if (m_SymbolsOffset == 0x0) {
    LOG_WR << "Unable to get imported symbols, symbol offset is not yet initialized" << LogEnd;
    return;
  }

  BinaryStream const&                    rBinStrm = rDoc.GetBinaryStream();
  typedef MachOTraits<bit>               MachOType;
  typename MachOType::DynamicSymbolTable DySymTab;

  /* Read Dynamic Symbol Table */
  if (!rBinStrm.Read(LoadCmdOff, &DySymTab, sizeof(DySymTab))) {
    LOG_WR << "Cannot read dysymtab command" << LogEnd;
    return;
  }

  u64 ImpAddr = m_ImportsAddress;
  for (u32 i = m_SymbolsIndex; i < DySymTab.nindirectsyms; ++i, ImpAddr += (bit / 8))
  {
    u32 SymIdx;

    if (!rBinStrm.Read(DySymTab.indirectsymoff + sizeof(SymIdx) * i, SymIdx)) {
      LOG_WR << "Cannot read symbol index" << LogEnd;
    }

    typename MachOType::Symbol Sym;

    /* Read Symbol */
    if (!rBinStrm.Read(m_SymbolsOffset + sizeof(Sym) * SymIdx, &Sym, sizeof(Sym))) {
      LOG_WR << "Cannot read the symbol " << i << LogEnd;
      continue;
    }

    std::string SymName;

    /* Read Symbol Name */
    if (!rBinStrm.Read(m_StringsOffset + Sym.n_strx, SymName)) {
      LOG_WR << "Cannot read the symbol name " << i << LogEnd;
      continue;
    }

    rDoc.AddLabel(
      Address(Address::FlatType, 0x0, ImpAddr, 0x10, bit),
      Label(SymName, Label::Imported | Label::Data));
  }
}

void MachOLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
{
  std::string ArchName;

  switch (m_Machine)
  {
  case CPU_TYPE_X86:
  case CPU_TYPE_X86_64: ArchName = "Intel x86"; break;
  case CPU_TYPE_ARM:    ArchName = "ARM";       break;
  default:                                      return;
  }

  rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [&ArchName](Architecture::SPType spArch)
  { return spArch->GetName() != ArchName; }), std::end(rArchs));
}

bool MachOLoader::_FindArchitectureTagAndModeByMachine(
  Architecture::VSPType const& rArchs,
  Tag& rArchTag, u8& rArchMode) const
{
  std::string ArchTagName, ArchModeName;

  switch (m_Machine)
  {
  case CPU_TYPE_X86:
    ArchTagName  = "Intel x86";
    ArchModeName = "32-bit";
    break;

  case CPU_TYPE_X86_64:
    ArchTagName  = "Intel x86";
    ArchModeName = "64-bit";
    break;

  case CPU_TYPE_ARM:
    ArchTagName = "ARM";
    break;
  }

  for (auto spArch : rArchs)
  {

    if (ArchTagName == spArch->GetName())
    {
      rArchTag  = spArch->GetTag();
      rArchMode = spArch->GetModeByName(ArchModeName);
      return true;
    }
  }

  return false;
}