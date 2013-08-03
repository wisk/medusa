#ifndef ELF_INTERPRETER_HPP
#define ELF_INTERPRETER_HPP

#include <medusa/medusa.hpp>
#include <medusa/document.hpp>
#include <medusa/function.hpp>
#include <medusa/log.hpp>

#include "elf_traits.hpp"

#include <vector>
#include <sstream>

#include <boost/foreach.hpp>

template<int n> class ElfInterpreter
{
public:
  ElfInterpreter(Document& rDoc, EEndianness Endianness)
    : m_rDoc(rDoc)
    , m_Endianness(Endianness)
    , m_pShStrTbl(nullptr)
  {
    m_rDoc.GetFileBinaryStream().Read(0x0, &m_Ehdr, sizeof(m_Ehdr));
    TElfType::EndianSwap(m_Ehdr, Endianness);
  }

  ~ElfInterpreter(void)
  {
    delete[] m_pShStrTbl;
  }

  Address GetEntryPoint(void)
  {
    return Address(Address::FlatType, 0x0, m_Ehdr.e_entry, 0x10, n);;
  }

  void Map(void)
  {
    BinaryStream const& rBinStrm = m_rDoc.GetFileBinaryStream();
    std::vector<typename TElfType::Shdr*> Sections;
    std::vector<typename TElfType::Phdr*> Segments;

    Sections.reserve(m_Ehdr.e_shnum);
    Segments.reserve(m_Ehdr.e_phnum);

    typename TElfType::Shdr* pShStrShdr = nullptr;

    // Are we lucky enough to have section header?
    // We must be very carefull since Section header are not mandatory to load an executable
    // in fact, the kernel doesn't even bother reading it.
    if (m_Ehdr.e_shoff != 0x0)
    {
      // Retrieve section header string tables
      pShStrShdr = new typename TElfType::Shdr;
      rBinStrm.Read(m_Ehdr.e_shoff + sizeof(*pShStrShdr) * m_Ehdr.e_shstrndx, pShStrShdr, sizeof(*pShStrShdr));
      TElfType::EndianSwap(*pShStrShdr, m_Endianness);

      if (pShStrShdr->sh_size > rBinStrm.GetSize())
        Log::Write("ldr_elf") << "Section string size is corrupted" << LogEnd;
      else
      {
        m_pShStrTbl = new char[static_cast<u32>(pShStrShdr->sh_size)];
        rBinStrm.Read(pShStrShdr->sh_offset, m_pShStrTbl, static_cast<u32>(pShStrShdr->sh_size));
      }

      // Read all section headers
      for (typename TElfType::Half Scn = 0; Scn < m_Ehdr.e_shnum; ++Scn)
      {
        typename TElfType::Shdr* pShdr = new typename TElfType::Shdr;

        rBinStrm.Read(m_Ehdr.e_shoff + sizeof(*pShdr) * Scn, pShdr, sizeof(*pShdr));
        if (pShdr->sh_size == 0)
          continue;
        TElfType::EndianSwap(*pShdr, m_Endianness);
        Log::Write("ldr_elf") << "Section found"
          << ": va="     << pShdr->sh_addr
          << ", offset=" << pShdr->sh_offset
          << ", size="   << pShdr->sh_size
          << ", name="   << m_pShStrTbl + pShdr->sh_name
          << LogEnd;

        if (pShdr->sh_addr == 0x0)
          continue;
        Sections.push_back(pShdr);
      }
    }

    // Are we lucky enough to have program segment header ?
    if (m_Ehdr.e_phoff != 0x0)
    {
      // Real all program segment headers
      for (typename TElfType::Half Segment = 0; Segment < m_Ehdr.e_phnum; ++Segment)
      {
        typename TElfType::Phdr* pPhdr = new typename TElfType::Phdr;

        rBinStrm.Read(m_Ehdr.e_phoff + sizeof(*pPhdr) * Segment, pPhdr, sizeof(*pPhdr));

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
      BOOST_FOREACH(typename TElfType::Shdr* pShdr, Sections)
      {
        char const* ShName =
          pShStrShdr && pShdr->sh_name > pShStrShdr->sh_size ?
          "" : m_pShStrTbl + pShdr->sh_name;

        u32 MemAreaFlags = MA_READ;

        if (pShdr->sh_flags & SHF_WRITE)
          MemAreaFlags |= MA_WRITE;
        if (pShdr->sh_flags & SHF_EXECINSTR)
          MemAreaFlags |= MA_EXEC;

        m_rDoc.AddMemoryArea(new MappedMemoryArea(
          m_rDoc.GetFileBinaryStream(), ShName,
          Address(Address::PhysicalType, 0x0, pShdr->sh_offset),  static_cast<u32>(pShdr->sh_size),
          Address(Address::FlatType, 0x0, pShdr->sh_addr, 16, n), static_cast<u32>(pShdr->sh_size),
          MemAreaFlags
          ));
      }
    }

    // Executable / shared object
    else
    {
      if (m_Ehdr.e_entry == 0)
        Log::Write("ldr_elf") << "Shared object" << LogEnd;
      else
        Log::Write("ldr_elf") << "Executable" << LogEnd;

      // We try to use section information since there are more useful than segment
      // LATER: At this time, we suppose it's better to use sections info instead of segments info if we've more sections than segments.
      // It could be better to let the user decide.
      if (Sections.size() >= Segments.size())
      {
        Log::Write("ldr_elf") << "Relocatable object" << LogEnd;
        BOOST_FOREACH(typename TElfType::Shdr* pShdr, Sections)
        {
          char const* ShName =
            pShStrShdr && pShdr->sh_name > pShStrShdr->sh_size ?
            "" : m_pShStrTbl + pShdr->sh_name;

          u32 MemAreaFlags = MA_READ;

          if (pShdr->sh_flags & SHF_WRITE)
            MemAreaFlags |= MA_WRITE;
          if (pShdr->sh_flags & SHF_EXECINSTR)
            MemAreaFlags |= MA_EXEC;

          if (pShdr->sh_type == SHT_NOBITS)
          {
            m_rDoc.AddMemoryArea(new VirtualMemoryArea(
              m_rDoc.GetFileBinaryStream().GetEndianness(), ShName,
              Address(Address::FlatType, 0x0, pShdr->sh_addr, 16, n), static_cast<u32>(pShdr->sh_size),
              MemAreaFlags
              ));
          }
          else
          {
            m_rDoc.AddMemoryArea(new MappedMemoryArea(
              m_rDoc.GetFileBinaryStream(), ShName,
              Address(Address::PhysicalType, 0x0, pShdr->sh_offset),  static_cast<u32>(pShdr->sh_size),
              Address(Address::FlatType, 0x0, pShdr->sh_addr, 16, n), static_cast<u32>(pShdr->sh_size),
              MemAreaFlags
              ));
          }
        }
      }

      else
      {
        u32 PhdrNo = 0;
        BOOST_FOREACH(typename TElfType::Phdr* pPhdr, Segments)
        {
          u32 MemAreaFlags = 0x0;

          if (pPhdr->p_flags & PF_X)
            MemAreaFlags |= MA_EXEC;
          if (pPhdr->p_flags & PF_W)
            MemAreaFlags |= MA_WRITE;
          if (pPhdr->p_flags & PF_R)
            MemAreaFlags |= MA_READ;

          std::ostringstream ShName;
          ShName << "phdr" << PhdrNo++;

          m_rDoc.AddMemoryArea(new MappedMemoryArea(
                m_rDoc.GetFileBinaryStream(), ShName.str(),
                Address(Address::PhysicalType, 0x0, pPhdr->p_offset),        static_cast<u32>(pPhdr->p_filesz),
                Address(Address::FlatType, 0x0, pPhdr->p_vaddr, 16, n), static_cast<u32>(pPhdr->p_memsz),
                MemAreaFlags
                ));
        }
      }
    }

    /* Try to retrieve imported function */
    BOOST_FOREACH(typename TElfType::Phdr* pPhdr, Segments)
    {
      typename TElfType::Addr SymTbl    = 0x0;
      typename TElfType::Addr JmpRelTbl = 0x0;
      typename TElfType::Addr DynStr    = 0x0;
      typename TElfType::Addr RelaTbl   = 0x0;
      u32 SymSz                         = 0;
      u32 JmpRelSz                      = 0;
      u32 DynStrSz                      = 0;
      u32 RelaSz                        = 0;
      u32 PltRelType                    = 0;

      if (pPhdr->p_type == PT_DYNAMIC)
      {
        u8* pDynamic = new u8[pPhdr->p_filesz];
        rBinStrm.Read(pPhdr->p_offset, pDynamic, pPhdr->p_filesz);

        for (typename TElfType::Dyn *pDyn = reinterpret_cast<typename TElfType::Dyn*>(pDynamic);
          reinterpret_cast<u8*>(pDyn) < pDynamic + pPhdr->p_filesz; ++pDyn)
        {
          TElfType::EndianSwap(*pDyn, m_Endianness);
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
        m_rDoc.Translate(Address(Address::FlatType, 0x0, SymTbl),    SymTblOff);
        m_rDoc.Translate(Address(Address::FlatType, 0x0, JmpRelTbl), JmpRelTblOff);
        m_rDoc.Translate(Address(Address::FlatType, 0x0, DynStr),    DynStrOff);
        m_rDoc.Translate(Address(Address::FlatType, 0x0, RelaTbl),   RelaTblOff);

        u8*   pReloc      = new u8[JmpRelSz];
        char* pDynSymStr  = new char[DynStrSz];
        u8*   pRelocA     = new u8[RelaSz];

        rBinStrm.Read(JmpRelTblOff, pReloc,     JmpRelSz);
        rBinStrm.Read(DynStrOff,    pDynSymStr, DynStrSz);
        rBinStrm.Read(RelaTblOff,   pRelocA,    RelaSz  );

        // XXX: At this time, it's unclear if we R_XXX_JMP_SLOT is always a Rel, Rela or both
        // We assume that it's Rela for now
        if (pReloc != nullptr && JmpRelSz != 0x0)
        {
          if (PltRelType == DT_REL)
          {
            Log::Write("ldr_elf") << "PLt relocs are Rel" << LogEnd;
            typename TElfType::Rel* pRel = reinterpret_cast<typename TElfType::Rel*>(pReloc);
            for (u32 i = 0; i < JmpRelSz / sizeof(*pRel); ++pRel, ++i)
            {
              TElfType::EndianSwap(*pRel, m_Endianness);

              typename TElfType::Sym CurSym;
              u32 SymIdx;
              if (n == 32)
                SymIdx = static_cast<u32>(pRel->r_info >> 8);
              else
                SymIdx = static_cast<u64>(pRel->r_info) >> 32;
              TOffset CurSymOff = SymTblOff + SymIdx * sizeof(CurSym);

              rBinStrm.Read(CurSymOff, &CurSym, sizeof(CurSym));
              TElfType::EndianSwap(CurSym, m_Endianness);
              if (pDynSymStr[CurSym.st_name] == '\0')
                continue;

              TOffset FuncOff;
              m_rDoc.Translate(pRel->r_offset, FuncOff);

              typename TElfType::Addr FuncPlt;
              rBinStrm.Read(FuncOff, FuncPlt);

              Address FuncPltAddr(Address::FlatType, 0x0, FuncPlt, 0, n);

              Log::Write("ldr_elf")
                << "Symbol found"
                << ": address=" << pRel->r_offset
                << ", plt=" << FuncPlt
                << ", name=" << pDynSymStr + CurSym.st_name
                << LogEnd;

              Address FuncAddr(Address::FlatType, 0x0, static_cast<TOffset>(pRel->r_offset), 0, n);
              std::string FuncName(pDynSymStr + CurSym.st_name);

              m_rDoc.AddLabel(FuncAddr, Label(FuncName, Label::Data | Label::Imported | Label::Global));
              m_rDoc.AddLabel(FuncPltAddr, Label(FuncName + "@plt", Label::Code | Label::Global));
            }
          }
          else if (PltRelType == DT_RELA)
          {
            Log::Write("ldr_elf") << "PLt relocs are Rela" << LogEnd;
            typename TElfType::Rela* pRela = reinterpret_cast<typename TElfType::Rela*>(pReloc);
            for (u32 i = 0; i < JmpRelSz / sizeof(*pRela); ++pRela, ++i)
            {
              TElfType::EndianSwap(*pRela, m_Endianness);

              typename TElfType::Sym CurSym;
              u32 SymIdx;
              if (n == 32)
                SymIdx = static_cast<u32>(pRela->r_info >> 8);
              else
                SymIdx = static_cast<u64>(pRela->r_info) >> 32;
              TOffset CurSymOff = SymTblOff + SymIdx * sizeof(CurSym);

              rBinStrm.Read(CurSymOff, &CurSym, sizeof(CurSym));

              if (pDynSymStr[CurSym.st_name] == '\0')
                continue;

              TElfType::EndianSwap(CurSym, m_Endianness);

              TOffset FuncOff;
              m_rDoc.Translate(pRela->r_offset, FuncOff);

              typename TElfType::Addr FuncPlt;
              rBinStrm.Read(FuncOff, FuncPlt);

              FuncPlt &= ~0xf;

              Log::Write("ldr_elf")
                << "Symbol found"
                << ": address=" << pRela->r_offset
                << ", plt=" << FuncPlt
                << ", name=" << pDynSymStr + CurSym.st_name
                << LogEnd;

              Address FuncAddr(Address::FlatType, 0x0, static_cast<TOffset>(pRela->r_offset), 0x10, n);
              std::string FuncName(pDynSymStr + CurSym.st_name);

              m_rDoc.AddLabel(FuncAddr, Label(FuncName, Label::Data | Label::Imported | Label::Global));
              m_rDoc.AddLabel(FuncPlt, Label(FuncName + "@plt", Label::Code | Label::Global));
              //m_rDoc.InsertMultiCell(FuncPlt, new Function);
            }
          } // if (PltRelType == DT_REL)
        } // if (pReloc != 0x0 && JmpRelSz != 0x0)

        if (pRelocA != nullptr && RelaSz)
        {
          typename TElfType::Rela* pRela = reinterpret_cast<typename TElfType::Rela*>(pRelocA);
          for (u32 i = 0; i < RelaSz / sizeof(*pRela); ++i, ++pRela)
          {
            TElfType::EndianSwap(*pRela, m_Endianness);

            typename TElfType::Sym CurSym;
            u32     SymIdx    = pRela->r_info >> (sizeof(pRela->r_info) * 8 / 2);
            TOffset CurSymOff = SymTblOff + SymIdx * sizeof(CurSym);

            rBinStrm.Read(CurSymOff, &CurSym, sizeof(CurSym));

            TElfType::EndianSwap(CurSym, m_Endianness);

            Log::Write("ldr_elf")
              << "Symbol found"
              << ": address=" << pRela->r_offset
              << ", name=" << pDynSymStr + CurSym.st_name
              << LogEnd;

            Address SymAddr(Address::FlatType, 0x0, static_cast<TOffset>(pRela->r_offset), 0x10, n);
            std::string SymName(pDynSymStr + CurSym.st_name);

            // TODO: Use ELFXX_ST_TYPE instead
            if ((CurSym.st_info & 0xf) == STT_FUNC)
              m_rDoc.AddLabel(SymAddr, Label(SymName, Label::Code | Label::Exported | Label::Global));
            else
              m_rDoc.AddLabel(SymAddr, Label(SymName, Label::Data | Label::Exported | Label::Global));
          }

          delete [] pDynSymStr;
          delete [] pReloc;
          delete [] pRelocA;
        }
      }
    }

    /* Clean the whole thing */
    BOOST_FOREACH(typename TElfType::Shdr* pShdr, Sections)
      delete pShdr;

    BOOST_FOREACH(typename TElfType::Phdr* pPhdr, Segments)
      delete pPhdr;

    delete pShStrShdr;
  }

private:
  Document&               m_rDoc;
  EEndianness             m_Endianness;
  typedef ElfTraits<n>    TElfType;
  typename TElfType::Ehdr m_Ehdr;
  char*                   m_pShStrTbl;
};

#endif // !ELF_INTERPRETER_HPP
