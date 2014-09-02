#ifndef LDR_ELF_HPP
#define LDR_ELF_HPP

#include <medusa/namespace.hpp>
#include <medusa/document.hpp>
#include <medusa/loader.hpp>
#include <medusa/log.hpp>

#include "elf.h"
#include "elf_traits.hpp"

#include <boost/foreach.hpp> // TODO: Use c++11 foreach instead of BOOST_FOREACH

MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_elf_EXPORTS
#  define LDR_ELF_EXPORT __declspec(dllexport)
#else
#  define LDR_ELF_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_ELF_EXPORT
#endif

class ElfLoader : public Loader
{
public:
  ElfLoader(void);

  virtual std::string GetName(void) const;
  virtual u8          GetDepth(void) const { return 1; }
  virtual bool        IsCompatible(BinaryStream const& rBinStrm);
  virtual void        Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs);
  virtual void        FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const;

private:
  u8  m_Ident[EI_NIDENT];
  u16 m_Machine;

  bool FindArchitectureTagAndModeByMachine(
      Architecture::VectorSharedPtr const& rArchs,
      Tag& rArchTag,
      u8&  rArchMode
      ) const;

  // TODO: Move and clean this function
  template<int bit> void Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs) // TODO: Use unique_ptr instead of new/delete to avoid memleak in case of exception
  {
    if (rArchs.empty())
      return;

    Tag ArchTag;
    u8  ArchMode;

    if (!FindArchitectureTagAndModeByMachine(rArchs, ArchTag, ArchMode))
      return;

    BinaryStream const& rBinStrm = rDoc.GetBinaryStream();

    EEndianness Endianness;
    switch (m_Ident[EI_DATA])
    {
    case ELFDATA2LSB: Endianness = LittleEndian; break;
    case ELFDATA2MSB: Endianness = BigEndian;    break;
    default:
      return;
    }

    typedef ElfTraits<bit> ElfType;
    typename ElfType::Ehdr Ehdr;
    typename ElfType::Shdr* pShStrShdr = nullptr;
    char* pShStrTbl;

    if (!rBinStrm.Read(0x0, &Ehdr, sizeof(Ehdr)))
    {
      Log::Write("ldr_elf") << "Can't read EHDR" << LogEnd;
      return;
    }
    ElfType::EndianSwap(Ehdr, Endianness);

    std::vector<typename ElfType::Shdr*> Sections;
    std::vector<typename ElfType::Phdr*> Segments;

    Sections.reserve(Ehdr.e_shnum);
    Segments.reserve(Ehdr.e_phnum);


    // Are we lucky enough to have section header?
    // We must be very carefull since Section header are not mandatory to load an executable
    // in fact, the kernel doesn't even bother reading it.
    if (Ehdr.e_shoff != 0x0)
    {
      // Retrieve section header string tables
      pShStrShdr = new typename ElfType::Shdr;
      if (!rBinStrm.Read(Ehdr.e_shoff + sizeof(*pShStrShdr) * Ehdr.e_shstrndx, pShStrShdr, sizeof(*pShStrShdr)))
      {
        Log::Write("ldr_elf") << "Can't read SHSTR" << LogEnd;
        return;
      }

      rDoc.AddLabel(Address(Address::FlatType, 0x0, Ehdr.e_entry, 0x10, bit), Label("start", Label::Code | Label::Exported));

      ElfType::EndianSwap(*pShStrShdr, Endianness);

      if (pShStrShdr->sh_size > rBinStrm.GetSize())
        Log::Write("ldr_elf") << "Section string size is corrupted" << LogEnd;
      else
      {
        pShStrTbl = new char[static_cast<u32>(pShStrShdr->sh_size)];
        if (!rBinStrm.Read(pShStrShdr->sh_offset, pShStrTbl, static_cast<u32>(pShStrShdr->sh_size)))
        {
          Log::Write("ldr_elf") << "Can't read string SHDR" << LogEnd;
          return;
        }
      }

      // Read all section headers
      for (typename ElfType::Half Scn = 0; Scn < Ehdr.e_shnum; ++Scn)
      {
        typename ElfType::Shdr* pShdr = new typename ElfType::Shdr;

        if (!rBinStrm.Read(Ehdr.e_shoff + sizeof(*pShdr) * Scn, pShdr, sizeof(*pShdr)))
        {
          Log::Write("ldr_elf") << "Can't read SHDR" << LogEnd;
          continue;
        }
        if (pShdr->sh_size == 0)
          continue;
        ElfType::EndianSwap(*pShdr, Endianness);
        Log::Write("ldr_elf") << "Section found"
          << ": va="     << pShdr->sh_addr
          << ", offset=" << pShdr->sh_offset
          << ", size="   << pShdr->sh_size
          << ", name="   << pShStrTbl + pShdr->sh_name
          << LogEnd;

        if (pShdr->sh_addr == 0x0)
          continue;
        Sections.push_back(pShdr);
      }
    }

    // Are we lucky enough to have program segment header?
    if (Ehdr.e_phoff != 0x0)
    {
      // Real all program segment headers
      for (typename ElfType::Half Segment = 0; Segment < Ehdr.e_phnum; ++Segment)
      {
        typename ElfType::Phdr* pPhdr = new typename ElfType::Phdr;

        if (!rBinStrm.Read(Ehdr.e_phoff + sizeof(*pPhdr) * Segment, pPhdr, sizeof(*pPhdr)))
        {
          Log::Write("ldr_elf") << "Can't read PHDR" << LogEnd;
          continue;
        }

        // At this time, we only want PT_LOAD and PT_DYNAMIC type
        if (pPhdr->p_type != PT_LOAD && pPhdr->p_type != PT_DYNAMIC)
        {
          delete pPhdr;
          continue;
        }

        Segments.push_back(pPhdr);
        Log::Write("ldr_elf") << "Segment found"
          << ": va="      << pPhdr->p_vaddr
          << ", offset=" << pPhdr->p_offset
          << ", memsz="  << pPhdr->p_memsz
          << LogEnd;
      }
    }

    Log::Write("ldr_elf") << "Number of section: " << Sections.size() << LogEnd;
    Log::Write("ldr_elf") << "Number of segment: " << Segments.size() << LogEnd;

    // Invalid executable ?
    if (Segments.size() == 0 && Sections.size() == 0)
    {
      Log::Write("ldr_elf") << "Can't find either segment or section" << LogEnd;
    }

    // Relocatable
    else if (Segments.size() == 0)
    {
      Log::Write("ldr_elf") << "Relocatable object" << LogEnd;
      BOOST_FOREACH(typename ElfType::Shdr* pShdr, Sections)
      {
        char const* ShName =
          pShStrShdr && pShdr->sh_name > pShStrShdr->sh_size ?
          "" : pShStrTbl + pShdr->sh_name;

        u32 MemAreaFlags = MemoryArea::Read;

        if (pShdr->sh_flags & SHF_WRITE)
          MemAreaFlags |= MemoryArea::Write;
        if (pShdr->sh_flags & SHF_EXECINSTR)
          MemAreaFlags |= MemoryArea::Execute;

        rDoc.AddMemoryArea(new MappedMemoryArea(
          ShName,
          0x0,  static_cast<u32>(pShdr->sh_size),
          Address(Address::FlatType, 0x0, pShdr->sh_addr, 16, bit), static_cast<u32>(pShdr->sh_size),
          MemAreaFlags,
          ArchTag, ArchMode
          ));
      }
    }

    // Executable / shared object
    else
    {
      if (Ehdr.e_entry == 0)
        Log::Write("ldr_elf") << "Shared object" << LogEnd;
      else
        Log::Write("ldr_elf") << "Executable" << LogEnd;

      // We try to use section information since there are more useful than segment
      // LATER: At this time, we suppose it's better to use sections info instead of segments info if we've more sections than segments.
      // It could be better to let the user decide.
      if (Sections.size() >= Segments.size())
      {
        Log::Write("ldr_elf") << "Relocatable object" << LogEnd;
        BOOST_FOREACH(typename ElfType::Shdr* pShdr, Sections)
        {
          char const* ShName =
            pShStrShdr && pShdr->sh_name > pShStrShdr->sh_size ?
            "" : pShStrTbl + pShdr->sh_name;

          u32 MemAreaFlags = MemoryArea::Read;

          if (pShdr->sh_flags & SHF_WRITE)
            MemAreaFlags |= MemoryArea::Write;
          if (pShdr->sh_flags & SHF_EXECINSTR)
            MemAreaFlags |= MemoryArea::Execute;

          if (pShdr->sh_type == SHT_NOBITS)
          {
            rDoc.AddMemoryArea(new VirtualMemoryArea(
              ShName,
              Address(Address::FlatType, 0x0, pShdr->sh_addr, 16, bit), static_cast<u32>(pShdr->sh_size),
              MemAreaFlags,
              ArchMode, ArchMode
              ));
          }
          else
          {
            rDoc.AddMemoryArea(new MappedMemoryArea(
              ShName,
              pShdr->sh_offset, static_cast<u32>(pShdr->sh_size),
              Address(Address::FlatType, 0x0, pShdr->sh_addr, 16, bit), static_cast<u32>(pShdr->sh_size),
              MemAreaFlags,
              ArchTag, ArchMode
              ));
          }
        }
      }

      else
      {
        u32 PhdrNo = 0;
        BOOST_FOREACH(typename ElfType::Phdr* pPhdr, Segments)
        {
          u32 MemAreaFlags = 0x0;

          if (pPhdr->p_flags & PF_X)
            MemAreaFlags |= MemoryArea::Execute;
          if (pPhdr->p_flags & PF_W)
            MemAreaFlags |= MemoryArea::Write;
          if (pPhdr->p_flags & PF_R)
            MemAreaFlags |= MemoryArea::Read;

          std::ostringstream ShName;
          ShName << "phdr" << PhdrNo++;

          rDoc.AddMemoryArea(new MappedMemoryArea(
                ShName.str(),
                pPhdr->p_offset, static_cast<u32>(pPhdr->p_filesz),
                Address(Address::FlatType, 0x0, pPhdr->p_vaddr, 16, bit), static_cast<u32>(pPhdr->p_memsz),
                MemAreaFlags,
                ArchTag, ArchMode
                ));
        }
      }
    }

    /* Try to retrieve imported function */
    BOOST_FOREACH(typename ElfType::Phdr* pPhdr, Segments)
    {
      typename ElfType::Addr SymTbl    = 0x0;
      typename ElfType::Addr JmpRelTbl = 0x0;
      typename ElfType::Addr DynStr    = 0x0;
      typename ElfType::Addr RelaTbl   = 0x0;
      u32 SymSz                         = 0;
      u32 JmpRelSz                      = 0;
      u32 DynStrSz                      = 0;
      u32 RelaSz                        = 0;
      u32 PltRelType                    = 0;

      if (pPhdr->p_type == PT_DYNAMIC)
      {
        u8* pDynamic = new u8[pPhdr->p_filesz];
        if (!rBinStrm.Read(pPhdr->p_offset, pDynamic, pPhdr->p_filesz))
        {
          Log::Write("ldr_elf") << "Can't read DYN" << LogEnd;
          continue;
        }

        for (typename ElfType::Dyn *pDyn = reinterpret_cast<typename ElfType::Dyn*>(pDynamic);
          reinterpret_cast<u8*>(pDyn) < pDynamic + pPhdr->p_filesz; ++pDyn)
        {
          ElfType::EndianSwap(*pDyn, Endianness);
          switch (pDyn->d_tag)
          {
          case DT_JMPREL:   JmpRelTbl   = pDyn->d_un.d_ptr;                   break;
          case DT_PLTRELSZ: JmpRelSz    = static_cast<u32>(pDyn->d_un.d_val); break;

          case DT_RELA:     RelaTbl     = pDyn->d_un.d_ptr;                   break;
          case DT_RELASZ:   RelaSz      = static_cast<u32>(pDyn->d_un.d_val); break;

          case DT_SYMTAB:   SymTbl      = pDyn->d_un.d_ptr;                   break;
          case DT_SYMENT:   SymSz       = static_cast<u32>(pDyn->d_un.d_val); break;

          case DT_STRTAB:   DynStr      = pDyn->d_un.d_ptr;                   break;
          case DT_STRSZ:    DynStrSz    = static_cast<u32>(pDyn->d_un.d_val); break;

          case DT_PLTREL:   PltRelType  = static_cast<u32>(pDyn->d_un.d_val); break;
          default:                                                            break;
          }
        }

        delete[] pDynamic;

        if (SymTbl == 0x0 || JmpRelTbl == 0x0)
          break;

        TOffset SymTblOff     = 0x0;
        TOffset JmpRelTblOff  = 0x0;
        TOffset DynStrOff     = 0x0;
        TOffset RelaTblOff    = 0x0;
        rDoc.ConvertAddressToFileOffset(Address(Address::FlatType, 0x0, SymTbl),    SymTblOff);
        rDoc.ConvertAddressToFileOffset(Address(Address::FlatType, 0x0, JmpRelTbl), JmpRelTblOff);
        rDoc.ConvertAddressToFileOffset(Address(Address::FlatType, 0x0, DynStr),    DynStrOff);
        rDoc.ConvertAddressToFileOffset(Address(Address::FlatType, 0x0, RelaTbl),   RelaTblOff);

        u8*   pReloc      = new u8[JmpRelSz];
        char* pDynSymStr  = new char[DynStrSz];
        u8*   pRelocA     = new u8[RelaSz];

        if (!rBinStrm.Read(JmpRelTblOff, pReloc, JmpRelSz))
        {
          Log::Write("ldr_elf") << "Can't read REL" << LogEnd;
          return;
        }
        if (!rBinStrm.Read(DynStrOff, pDynSymStr, DynStrSz))
        {
          Log::Write("ldr_elf") << "Can't read DYNSTR" << LogEnd;
          return;
        }
        if (!rBinStrm.Read(RelaTblOff, pRelocA, RelaSz))
        {
          Log::Write("ldr_elf") << "Can't read RELA" << LogEnd;
          return;
        }

        // XXX: At this time, it's unclear if we R_XXX_JMP_SLOT is always a Rel, Rela or both
        // We assume that it's Rela for now
        if (pReloc != nullptr && JmpRelSz != 0x0)
        {
          if (PltRelType == DT_REL)
          {
            Log::Write("ldr_elf") << "PLt relocs are Rel" << LogEnd;
            typename ElfType::Rel* pRel = reinterpret_cast<typename ElfType::Rel*>(pReloc);
            for (u32 i = 0; i < JmpRelSz / sizeof(*pRel); ++pRel, ++i)
            {
              ElfType::EndianSwap(*pRel, Endianness);

              typename ElfType::Sym CurSym;
              u32 SymIdx;
              if (bit == 32)
                SymIdx = static_cast<u32>(pRel->r_info >> 8);
              else
                SymIdx = static_cast<u64>(pRel->r_info) >> 32;
              TOffset CurSymOff = SymTblOff + SymIdx * sizeof(CurSym);

              if (!rBinStrm.Read(CurSymOff, &CurSym, sizeof(CurSym)))
              {
                Log::Write("ldr_elf") << "Can't read SYM" << LogEnd;
                continue;
              }
              ElfType::EndianSwap(CurSym, Endianness);
              if (pDynSymStr[CurSym.st_name] == '\0')
                continue;

              TOffset FuncOff;
              if (!rDoc.ConvertAddressToFileOffset(pRel->r_offset, FuncOff))
              {
                Log::Write("ldr_elf") << "Can't convert address of REL" << LogEnd;
                continue;
              }

              typename ElfType::Addr FuncPlt;
              if (!rBinStrm.Read(FuncOff, FuncPlt))
              {
                Log::Write("ldr_elf") << "Can't read FUNPLT" << LogEnd;
                continue;
              }

              Address FuncPltAddr(Address::FlatType, 0x0, FuncPlt, 0, bit);

              Log::Write("ldr_elf")
                << "Symbol found"
                << ": address=" << pRel->r_offset
                << ", plt=" << FuncPlt
                << ", name=" << pDynSymStr + CurSym.st_name
                << LogEnd;

              Address FuncAddr(Address::FlatType, 0x0, static_cast<TOffset>(pRel->r_offset), 0, bit);
              std::string FuncName(pDynSymStr + CurSym.st_name);

              rDoc.AddLabel(FuncAddr, Label(FuncName, Label::Data | Label::Imported));
              //rDoc.AddLabel(FuncPltAddr, Label(FuncName + "@plt", Label::Code | Label::Global));
            }
          }
          else if (PltRelType == DT_RELA)
          {
            Log::Write("ldr_elf") << "PLt relocs are Rela" << LogEnd;
            typename ElfType::Rela* pRela = reinterpret_cast<typename ElfType::Rela*>(pReloc);
            for (u32 i = 0; i < JmpRelSz / sizeof(*pRela); ++pRela, ++i)
            {
              ElfType::EndianSwap(*pRela, Endianness);

              typename ElfType::Sym CurSym;
              u32 SymIdx;
              if (bit == 32)
                SymIdx = static_cast<u32>(pRela->r_info >> 8);
              else
                SymIdx = static_cast<u64>(pRela->r_info) >> 32;
              TOffset CurSymOff = SymTblOff + SymIdx * sizeof(CurSym);

              if (!rBinStrm.Read(CurSymOff, &CurSym, sizeof(CurSym)))
              {
                Log::Write("ldr_elf") << "Can't read SYM" << LogEnd;
                continue;
              }

              if (pDynSymStr[CurSym.st_name] == '\0')
                continue;

              ElfType::EndianSwap(CurSym, Endianness);

              TOffset FuncOff;
              if (!rDoc.ConvertAddressToFileOffset(pRela->r_offset, FuncOff))
              {
                Log::Write("ldr_elf") << "Can't convert address of RELA" << LogEnd;
                continue;
              }

              typename ElfType::Addr FuncPlt;
              if (!rBinStrm.Read(FuncOff, FuncPlt))
              {
                Log::Write("ldr_elf") << "Can't read FUNPLT" << LogEnd;
                continue;
              }

              FuncPlt &= ~0xf; // TODO: Find a better way to do this...

              Log::Write("ldr_elf")
                << "Symbol found"
                << ": address=" << pRela->r_offset
                << ", plt=" << FuncPlt
                << ", name=" << pDynSymStr + CurSym.st_name
                << LogEnd;

              Address FuncAddr(Address::FlatType, 0x0, static_cast<TOffset>(pRela->r_offset), 0x10, bit);
              std::string FuncName(pDynSymStr + CurSym.st_name);

              rDoc.ChangeValueSize(FuncAddr, bit, true);
              rDoc.AddLabel(FuncAddr, Label(FuncName, Label::Data | Label::Imported));
              //rDoc.AddLabel(FuncPlt, Label(FuncName + "@plt", Label::Code | Label::Global));
              //rDoc.InsertMultiCell(FuncPlt, new Function);
            }
          } // if (PltRelType == DT_REL)
        } // if (pReloc != 0x0 && JmpRelSz != 0x0)

        if (pRelocA != nullptr && RelaSz)
        {
          typename ElfType::Rela* pRela = reinterpret_cast<typename ElfType::Rela*>(pRelocA);
          for (u32 i = 0; i < RelaSz / sizeof(*pRela); ++i, ++pRela)
          {
            ElfType::EndianSwap(*pRela, Endianness);

            typename ElfType::Sym CurSym;
            u32     SymIdx    = pRela->r_info >> (sizeof(pRela->r_info) * 8 / 2);
            TOffset CurSymOff = SymTblOff + SymIdx * sizeof(CurSym);

            if (!rBinStrm.Read(CurSymOff, &CurSym, sizeof(CurSym)))
            {
              Log::Write("ldr_elf") << "Can't read SYM" << LogEnd;
              continue;
            }

            ElfType::EndianSwap(CurSym, Endianness);

            Log::Write("ldr_elf")
              << "Symbol found"
              << ": address=" << pRela->r_offset
              << ", name=" << pDynSymStr + CurSym.st_name
              << LogEnd;

            Address SymAddr(Address::FlatType, 0x0, static_cast<TOffset>(pRela->r_offset), 0x10, bit);
            std::string SymName(pDynSymStr + CurSym.st_name);

            // TODO: Use ELFXX_ST_TYPE instead
            if ((CurSym.st_info & 0xf) == STT_FUNC)
              rDoc.AddLabel(SymAddr, Label(SymName, Label::Code | Label::Exported));
            else
              rDoc.AddLabel(SymAddr, Label(SymName, Label::Data | Label::Exported));
          }

          delete [] pDynSymStr;
          delete [] pReloc;
          delete [] pRelocA;
        }
      }
    }

    /* Clean the whole thing */
    BOOST_FOREACH(typename ElfType::Shdr* pShdr, Sections)
      delete pShdr;

    BOOST_FOREACH(typename ElfType::Phdr* pPhdr, Segments)
      delete pPhdr;

    delete pShStrShdr;
  }
};

extern "C" LDR_ELF_EXPORT Loader* GetLoader(void);

#endif // !LDR_ELF_HPP
