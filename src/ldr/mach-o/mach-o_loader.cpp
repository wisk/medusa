#include "mach-o.h"
#include "mach-o_loader.hpp"
#include "mach-o_traits.hpp"
#include <medusa/medusa.hpp>
#include <boost/foreach.hpp>

#define LOG_WR Log::Write("ldr_mach-o")

#define TEXT_SECTION "__TEXT,__text"

MachOLoader::MachOLoader(void) :
    m_Machine(0),
    m_Arch64(false),
    m_Endian(EndianUnknown),
    m_EntryPoint(0x0),
    m_EntryPointType(ENTRYPOINT_NONE),
    m_TextSectionVMAddr(0x0)
{
}

bool MachOLoader::IsCompatible(BinaryStream const& rBinStrm)
{
    typedef MachOTraits<32>         MachOType;
    typename MachOType::MachOHeader Header;

    if (!rBinStrm.Read(0x0, &Header, sizeof(Header))) {
        return false;
    }

    if (   Header.magic == MH_MAGIC
        || Header.magic == MH_MAGIC_64
        || Header.magic == FAT_MAGIC) {
        m_Endian = LittleEndian;

    } else if (   Header.magic == MH_CIGAM
               || Header.magic == MH_CIGAM_64
               || Header.magic == FAT_CIGAM) {
        m_Endian = BigEndian;

    } else {
        return false;
    }

    MachOType::EndianSwap(Header, m_Endian);

    /* TODO : Support Fat binaries */
    if (Header.magic == FAT_MAGIC) {
        return false;
    }

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

Architecture::SharedPtr MachOLoader::GetMainArchitecture(Architecture::VectorSharedPtr const& rArchitectures)
{
    std::string ArchName;

    switch (m_Machine)
    {
    case CPU_TYPE_X86:
    case CPU_TYPE_X86_64: ArchName = "Intel x86"; break;
    case CPU_TYPE_ARM:    ArchName = "ARM";       break;
    }

    if (!ArchName.empty()) {
        for (auto itArch = std::begin(rArchitectures); itArch != std::end(rArchitectures); ++itArch) {
            if ((*itArch)->GetName() == ArchName) {
                return *itArch;
            }
        }
    }
    return Architecture::SharedPtr();
}

void MachOLoader::Configure(Configuration& rCfg)
{
    rCfg.Set("Bit", m_Arch64 ? 64 : 32);
}

void MachOLoader::Map(Document& rDoc)
{
    if (m_Arch64) {
        Map<64>(rDoc);
    } else {
        Map<32>(rDoc);
    }
}

template<int bit>
void MachOLoader::Map(Document& rDoc)
{
    BinaryStream const&             rBinStrm = rDoc.GetBinaryStream();
    typedef MachOTraits<bit>        MachOType;
    typename MachOType::MachOHeader Header;
    int                             LoadCmdOff;

    if (!rBinStrm.Read(0x0, &Header, sizeof(Header))) {
        LOG_WR << "Cannot read mach-o header" << LogEnd;
        return;
    }
    MachOType::EndianSwap(Header, m_Endian);

    LoadCmdOff = sizeof(Header);
    for (int i = 0; i < Header.ncmds; i++) {
        typename MachOType::LoadCmd LoadCmd;

        if (!rBinStrm.Read(LoadCmdOff, &LoadCmd, sizeof(LoadCmd))) {
            LOG_WR << "Cannot read load command" << LogEnd;
            return;
        }
        MachOType::EndianSwap(LoadCmd, m_Endian);

        switch (LoadCmd.cmd) {
        case LC_SEGMENT:
        case LC_SEGMENT_64:
            MachOLoader::MapSegment<bit>(rDoc, LoadCmdOff);
            break;

        case LC_SYMTAB:
            MachOLoader::GetSymbols<bit>(rDoc, LoadCmdOff);
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
    if (m_EntryPointType == ENTRYPOINT_OFFSET) {
        if (m_TextSectionVMAddr) {
            m_EntryPoint = m_TextSectionVMAddr + m_EntryPoint;
        }
    }
    rDoc.AddLabel(Address(Address::FlatType, 0x0, m_EntryPoint, 0x10, bit),
                  Label("start", Label::Code | Label::Exported));
}

template<int bit>
void MachOLoader::MapSegment(Document& rDoc, int LoadCmdOff)
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

        if ((Section.flags & SECTION_TYPE) == S_ZEROFILL) {
            rDoc.AddMemoryArea(new VirtualMemoryArea(
                                   FullSectionName,
                                   Address(Address::FlatType, 0x0, Section.addr, 16, bit),
                                   Section.size,
                                   MemAreaFlags
                                   ));
        } else {
            rDoc.AddMemoryArea(new MappedMemoryArea(
                                   FullSectionName,
                                   Section.offset,
                                   Section.size,
                                   Address(Address::FlatType, 0x0, Section.addr, 16, bit),
                                   Section.size,
                                   MemAreaFlags
                                   ));
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
        typedef MachOTraits<32>            MachOType;
        typename MachOType::x86StateHeader x86StateHeader;
        typename MachOType::x86State       x86State;

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
        m_EntryPointType = ENTRYPOINT_VMADDR;
    }
    break;

    case CPU_TYPE_X86_64:
    {
        typedef MachOTraits<64>            MachOType;
        typename MachOType::x86StateHeader x86StateHeader;
        typename MachOType::x86State       x86State;

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
        m_EntryPointType = ENTRYPOINT_VMADDR;
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
    BinaryStream const&            rBinStrm = rDoc.GetBinaryStream();
    typedef MachOTraits<32>        MachOType;
    typename MachOType::EntryPoint EntryPoint;

    if (!rBinStrm.Read(LoadCmdOff, &EntryPoint, sizeof(EntryPoint))) {
        LOG_WR << "Cannot read entry point command" << LogEnd;
        return;
    }
    MachOType::EndianSwap(EntryPoint, m_Endian);

    m_EntryPoint     = EntryPoint.entryoff;
    m_EntryPointType = ENTRYPOINT_OFFSET;
}

template<int bit>
void MachOLoader::GetSymbols(Document& rDoc, int LoadCmdOff)
{
    BinaryStream const&            rBinStrm = rDoc.GetBinaryStream();
    typedef MachOTraits<bit>       MachOType;
    typename MachOType::SymTab     SymTab;
    typename MachOType::Symbol     Symbol;
    u32                            SymbolOff;
    char                          *pStrTbl;

    /* Read Symbol Table */
    if (!rBinStrm.Read(LoadCmdOff, &SymTab, sizeof(SymTab))) {
        LOG_WR << "Cannot read symtab command" << LogEnd;
        return;
    }
    MachOType::EndianSwap(SymTab, m_Endian);

    /* Read String Table */
    pStrTbl = new char[static_cast<u32>(SymTab.strsize)];
    if (!rBinStrm.Read(SymTab.stroff, pStrTbl, static_cast<u32>(SymTab.strsize))) {
        LOG_WR << "Cannot read string table" << LogEnd;
        return;
    }

    SymbolOff = SymTab.symoff;
    //printf("  Symbol table (%u elements:0x%08x):\n", SymTab.nsyms, SymTab.stroff);
    for (u32 i = 0; i < SymTab.nsyms; i++) {

        /* Read Symbol Entry */
        if (!rBinStrm.Read(SymbolOff, &Symbol, sizeof(Symbol))) {
            LOG_WR << "Cannot read symbol" << LogEnd;
            return;
        }
        MachOType::EndianSwap(Symbol, m_Endian);

        if (Symbol.n_strx && strlen(pStrTbl + Symbol.n_strx)) {
/*            printf(" %04d: %08llx: %s (type:%08x, sect:%08x, desc:%08x)\n",
                   i,
                   Symbol.n_value,
                   pStrTbl + Symbol.n_strx,
                   Symbol.n_type,
                   Symbol.n_sect,
                   Symbol.n_desc);*/
        }

        SymbolOff += sizeof(Symbol);
    }

    delete pStrTbl;
}
