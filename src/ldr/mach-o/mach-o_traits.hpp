#ifndef LDR_MACH_O_TRAITS_HPP
#define LDR_MACH_O_TRAITS_HPP

#include "mach-o.h"
#include <medusa/endian.hpp>

template<int N>
struct MachOTraits
{
};

template<>
struct MachOTraits<32>
{
    typedef mach_header          MachOHeader;
    typedef load_command         LoadCmd;
    typedef segment_command      Segment;
    typedef section              Section;
    typedef x86_state_hdr        x86StateHeader;
    typedef x86_thread_state32_t x86State;
    typedef entry_point_command  EntryPoint;
    typedef symtab_command       SymbolTable;
    typedef dysymtab_command     DynamicSymbolTable;
    typedef nlist                Symbol;

    static void EndianSwap(Symbol& Symbol, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(Symbol.n_strx);
        ::EndianSwap(Symbol.n_desc);
        ::EndianSwap(Symbol.n_value);
    }

    static void EndianSwap(SymbolTable& SymTab, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(SymTab.cmd);
        ::EndianSwap(SymTab.cmdsize);
        ::EndianSwap(SymTab.symoff);
        ::EndianSwap(SymTab.nsyms);
        ::EndianSwap(SymTab.stroff);
        ::EndianSwap(SymTab.strsize);
    }

    static void EndianSwap(DynamicSymbolTable& DySymTab, EEndianness Endianness)
    {
      if (!TestEndian(Endianness)) {
        return;
      }

      ::EndianSwap(DySymTab.cmd);
      ::EndianSwap(DySymTab.cmdsize);
      ::EndianSwap(DySymTab.ilocalsym);
      ::EndianSwap(DySymTab.nlocalsym);
      ::EndianSwap(DySymTab.iextdefsym);
      ::EndianSwap(DySymTab.nextdefsym);
      ::EndianSwap(DySymTab.iundefsym);
      ::EndianSwap(DySymTab.nundefsym);
      ::EndianSwap(DySymTab.tocoff);
      ::EndianSwap(DySymTab.ntoc);
      ::EndianSwap(DySymTab.modtaboff);
      ::EndianSwap(DySymTab.nmodtab);
      ::EndianSwap(DySymTab.extrefsymoff);
      ::EndianSwap(DySymTab.nextrefsyms);
      ::EndianSwap(DySymTab.indirectsymoff);
      ::EndianSwap(DySymTab.nindirectsyms);
      ::EndianSwap(DySymTab.extreloff);
      ::EndianSwap(DySymTab.nextrel);
      ::EndianSwap(DySymTab.locreloff);
      ::EndianSwap(DySymTab.nlocrel);
    }

    static void EndianSwap(EntryPoint& EntryPoint, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(EntryPoint.cmd);
        ::EndianSwap(EntryPoint.cmdsize);
        ::EndianSwap(EntryPoint.entryoff);
        ::EndianSwap(EntryPoint.stacksize);
    }

    static void EndianSwap(x86State& x86State, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(x86State.eax);
        ::EndianSwap(x86State.ebx);
        ::EndianSwap(x86State.ecx);
        ::EndianSwap(x86State.edx);
        ::EndianSwap(x86State.edi);
        ::EndianSwap(x86State.esi);
        ::EndianSwap(x86State.ebp);
        ::EndianSwap(x86State.esp);
        ::EndianSwap(x86State.ss);
        ::EndianSwap(x86State.eflags);
        ::EndianSwap(x86State.eip);
        ::EndianSwap(x86State.cs);
        ::EndianSwap(x86State.ds);
        ::EndianSwap(x86State.es);
        ::EndianSwap(x86State.fs);
        ::EndianSwap(x86State.gs);
    }

    static void EndianSwap(x86StateHeader& x86StateHeader, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(x86StateHeader.flavor);
        ::EndianSwap(x86StateHeader.count);
    }

    static void EndianSwap(Section& Section, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(Section.addr);
        ::EndianSwap(Section.size);
        ::EndianSwap(Section.offset);
        ::EndianSwap(Section.align);
        ::EndianSwap(Section.reloff);
        ::EndianSwap(Section.nreloc);
        ::EndianSwap(Section.flags);
        ::EndianSwap(Section.reserved1);
        ::EndianSwap(Section.reserved2);
    }

    static void EndianSwap(Segment& Segment, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(Segment.cmd);
        ::EndianSwap(Segment.cmdsize);
        ::EndianSwap(Segment.vmaddr);
        ::EndianSwap(Segment.vmsize);
        ::EndianSwap(Segment.fileoff);
        ::EndianSwap(Segment.filesize);
        ::EndianSwap(Segment.maxprot);
        ::EndianSwap(Segment.initprot);
        ::EndianSwap(Segment.nsects);
        ::EndianSwap(Segment.flags);
    }

    static void EndianSwap(LoadCmd& LoadCmd, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(LoadCmd.cmd);
        ::EndianSwap(LoadCmd.cmdsize);
    }

    static void EndianSwap(MachOHeader& Header, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(Header.magic);
        ::EndianSwap(Header.cputype);
        ::EndianSwap(Header.cpusubtype);
        ::EndianSwap(Header.filetype);
        ::EndianSwap(Header.ncmds);
        ::EndianSwap(Header.sizeofcmds);
        ::EndianSwap(Header.flags);
    }
};

template<>
struct MachOTraits<64>
{
    typedef mach_header_64       MachOHeader;
    typedef load_command         LoadCmd;
    typedef segment_command_64   Segment;
    typedef section_64           Section;
    typedef x86_state_hdr        x86StateHeader;
    typedef x86_thread_state64_t x86State;
    typedef entry_point_command  EntryPoint;
    typedef symtab_command       SymbolTable;
    typedef dysymtab_command     DynamicSymbolTable;
    typedef nlist_64             Symbol;

    static void EndianSwap(Symbol& Symbol, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(Symbol.n_strx);
        ::EndianSwap(Symbol.n_desc);
        ::EndianSwap(Symbol.n_value);
    }

    static void EndianSwap(SymbolTable& SymTab, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(SymTab.cmd);
        ::EndianSwap(SymTab.cmdsize);
        ::EndianSwap(SymTab.symoff);
        ::EndianSwap(SymTab.nsyms);
        ::EndianSwap(SymTab.stroff);
        ::EndianSwap(SymTab.strsize);
    }

    static void EndianSwap(DynamicSymbolTable& DySymTab, EEndianness Endianness)
    {
      if (!TestEndian(Endianness)) {
        return;
      }

      ::EndianSwap(DySymTab.cmd);
      ::EndianSwap(DySymTab.cmdsize);
      ::EndianSwap(DySymTab.ilocalsym);
      ::EndianSwap(DySymTab.nlocalsym);
      ::EndianSwap(DySymTab.iextdefsym);
      ::EndianSwap(DySymTab.nextdefsym);
      ::EndianSwap(DySymTab.iundefsym);
      ::EndianSwap(DySymTab.nundefsym);
      ::EndianSwap(DySymTab.tocoff);
      ::EndianSwap(DySymTab.ntoc);
      ::EndianSwap(DySymTab.modtaboff);
      ::EndianSwap(DySymTab.nmodtab);
      ::EndianSwap(DySymTab.extrefsymoff);
      ::EndianSwap(DySymTab.nextrefsyms);
      ::EndianSwap(DySymTab.indirectsymoff);
      ::EndianSwap(DySymTab.nindirectsyms);
      ::EndianSwap(DySymTab.extreloff);
      ::EndianSwap(DySymTab.nextrel);
      ::EndianSwap(DySymTab.locreloff);
      ::EndianSwap(DySymTab.nlocrel);
    }

    static void EndianSwap(EntryPoint& EntryPoint, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(EntryPoint.cmd);
        ::EndianSwap(EntryPoint.cmdsize);
        ::EndianSwap(EntryPoint.entryoff);
        ::EndianSwap(EntryPoint.stacksize);
    }

    static void EndianSwap(x86State& x86State, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(x86State.rax);
        ::EndianSwap(x86State.rbx);
        ::EndianSwap(x86State.rcx);
        ::EndianSwap(x86State.rdx);
        ::EndianSwap(x86State.rdi);
        ::EndianSwap(x86State.rsi);
        ::EndianSwap(x86State.rbp);
        ::EndianSwap(x86State.rsp);
        ::EndianSwap(x86State.r8);
        ::EndianSwap(x86State.r9);
        ::EndianSwap(x86State.r10);
        ::EndianSwap(x86State.r11);
        ::EndianSwap(x86State.r12);
        ::EndianSwap(x86State.r13);
        ::EndianSwap(x86State.r14);
        ::EndianSwap(x86State.r15);
        ::EndianSwap(x86State.rip);
        ::EndianSwap(x86State.rflags);
        ::EndianSwap(x86State.cs);
        ::EndianSwap(x86State.fs);
        ::EndianSwap(x86State.gs);
    }

    static void EndianSwap(x86StateHeader& x86StateHeader, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(x86StateHeader.flavor);
        ::EndianSwap(x86StateHeader.count);
    }

    static void EndianSwap(Section& Section, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(Section.addr);
        ::EndianSwap(Section.size);
        ::EndianSwap(Section.offset);
        ::EndianSwap(Section.align);
        ::EndianSwap(Section.reloff);
        ::EndianSwap(Section.nreloc);
        ::EndianSwap(Section.flags);
        ::EndianSwap(Section.reserved1);
        ::EndianSwap(Section.reserved2);
    }

    static void EndianSwap(Segment& Segment, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(Segment.cmd);
        ::EndianSwap(Segment.cmdsize);
        ::EndianSwap(Segment.vmaddr);
        ::EndianSwap(Segment.vmsize);
        ::EndianSwap(Segment.fileoff);
        ::EndianSwap(Segment.filesize);
        ::EndianSwap(Segment.maxprot);
        ::EndianSwap(Segment.initprot);
        ::EndianSwap(Segment.nsects);
        ::EndianSwap(Segment.flags);
    }

    static void EndianSwap(LoadCmd& LoadCmd, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(LoadCmd.cmd);
        ::EndianSwap(LoadCmd.cmdsize);
    }

    static void EndianSwap(MachOHeader& Header, EEndianness Endianness)
    {
        if (!TestEndian(Endianness)) {
            return;
        }

        ::EndianSwap(Header.magic);
        ::EndianSwap(Header.cputype);
        ::EndianSwap(Header.cpusubtype);
        ::EndianSwap(Header.filetype);
        ::EndianSwap(Header.ncmds);
        ::EndianSwap(Header.sizeofcmds);
        ::EndianSwap(Header.flags);
        ::EndianSwap(Header.reserved);
    }
};

#endif // LDR_MACH_O_TRAITS_HPP
