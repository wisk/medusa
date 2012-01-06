#ifndef ELF_INTERPRETER_HPP
#define ELF_INTERPRETER_HPP

#include <medusa/medusa.hpp>
#include <medusa/database.hpp>
#include <medusa/function.hpp>
#include "elf_traits.hpp"

#include <vector>
#include <sstream>

#include <boost/foreach.hpp>

template<int n> class ElfInterpreter
{
public:
  ElfInterpreter(Database& rDatabase, EEndianness Endianness)
    : m_rDatabase(rDatabase)
    , m_Endianness(Endianness)
    , m_pShStrTbl(NULL)
  {
    m_rDatabase.GetFileBinaryStream().Read(0x0, &m_Ehdr, sizeof(m_Ehdr));
    TElfType::EndianSwap(m_Ehdr, Endianness);
  }

  ~ElfInterpreter(void)
  {
    delete[] m_pShStrTbl;
  }

  Address GetEntryPoint(void)
  {
    return m_Ehdr.e_entry;
  }

  void Map(void)
  {
    BinaryStream const& rBinStrm = m_rDatabase.GetFileBinaryStream();
    std::vector<typename TElfType::Shdr*> Sections;
    std::vector<typename TElfType::Phdr*> Segments;

    Sections.reserve(m_Ehdr.e_shnum);
    Segments.reserve(m_Ehdr.e_phnum);

    typename TElfType::Shdr* pShStrShdr = NULL;

    // Are we lucky enough to have section header ?
    if (m_Ehdr.e_shoff != 0x0)
    {
      // Retrieve section header string tables
      pShStrShdr = new typename TElfType::Shdr;
      rBinStrm.Read(m_Ehdr.e_shoff + sizeof(*pShStrShdr) * m_Ehdr.e_shstrndx, pShStrShdr, sizeof(*pShStrShdr));
      TElfType::EndianSwap(*pShStrShdr, m_Endianness);

      if (pShStrShdr->sh_size > rBinStrm.GetSize())
      {
        Medusa::Log() << "Section string size is corrupted" << std::endl;
      }
      else
      {
        m_pShStrTbl = new char[static_cast<u32>(pShStrShdr->sh_size)];
        rBinStrm.Read(pShStrShdr->sh_offset, m_pShStrTbl, static_cast<u32>(pShStrShdr->sh_size));
      }

      Medusa::Log() << std::hex;
      // Read all section headers
      for (typename TElfType::Half Scn = 0; Scn < m_Ehdr.e_shnum; ++Scn)
      {
        typename TElfType::Shdr* pShdr = new typename TElfType::Shdr;

        rBinStrm.Read(m_Ehdr.e_shoff + sizeof(*pShdr) * Scn, pShdr, sizeof(*pShdr));
        TElfType::EndianSwap(*pShdr, m_Endianness);
        Medusa::Log() << "Section found:"
          << " name="    << m_pShStrTbl + pShdr->sh_name
          << ", va="     << pShdr->sh_addr
          << ", offset=" << pShdr->sh_offset
          << ", size="   << pShdr->sh_size
          << std::endl;
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
        Medusa::Log() << "Segment found:"
          << " va="      << pPhdr->p_vaddr
          << ", offset=" << pPhdr->p_offset
          << ", memsz="  << pPhdr->p_memsz
          << std::endl;
      }
    }

    Medusa::Log() << std::dec;
    Medusa::Log() << "Number of section: " << Sections.size() << std::endl;
    Medusa::Log() << "Number of segment: " << Segments.size() << std::endl;

    // Invalid executable ?
    if (Segments.size() == 0 && Sections.size() == 0)
    {
      Medusa::Log() << "Can't find either segment or section" << std::endl;
    }

    // Relocatable
    else if (Segments.size() == 0)
    {
      Medusa::Log() << "Relocatable object" << std::endl;
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

        m_rDatabase.AddMemoryArea(new MappedMemoryArea(
          m_rDatabase.GetFileBinaryStream(), ShName,
          Address(Address::PhysicalType, pShdr->sh_offset),  static_cast<u32>(pShdr->sh_size),
          Address(Address::FlatType, pShdr->sh_addr, 16, n), static_cast<u32>(pShdr->sh_size),
          MemAreaFlags
          ));
      }
    }

    // Executable / shared object
    else
    {
      if (m_Ehdr.e_entry == 0)
        Medusa::Log() << "Shared object" << std::endl;
      else
        Medusa::Log() << "Executable" << std::endl;

      // We try to use section information since there are more useful than segment
      // XXX: What's happen if part of mem is on a segment but not in sections ?
      // XXX: This part is still broken and need more work from memory area management
      //      if (Sections.size() != 0)
      //      {
      //        Medusa::Log() << "Relocatable object" << std::endl;
      //        BOOST_FOREACH(typename TElfType::Shdr* pShdr, Sections)
      //        {
      //          char const* ShName =
      //            pShStrShdr && pShdr->sh_name > pShStrShdr->sh_size ?
      //            "" : m_pShStrTbl + pShdr->sh_name;
      //
      //          u32 MemAreaFlags = MA_READ;
      //
      //          if (pShdr->sh_flags & SHF_WRITE)
      //            MemAreaFlags |= MA_WRITE;
      //          if (pShdr->sh_flags & SHF_EXECINSTR)
      //            MemAreaFlags |= MA_EXEC;
      //
      //          m_rDatabase.AddMemoryArea(new MappedMemoryArea(
      //                m_rDatabase.GetFileBinaryStream(), ShName,
      //                Address(Address::PhysicalType, pShdr->sh_offset),  static_cast<u32>(pShdr->sh_size),
      //                Address(Address::FlatType, pShdr->sh_addr, 16, n), static_cast<u32>(pShdr->sh_size),
      //                MemAreaFlags
      //                ));
      //        }
      //      }
      //
      //      else
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

          m_rDatabase.AddMemoryArea(new MappedMemoryArea(
            m_rDatabase.GetFileBinaryStream(), ShName.str(),
            Address(Address::PhysicalType, pPhdr->p_offset),        static_cast<u32>(pPhdr->p_filesz),
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
        m_rDatabase.Translate(Address(SymTbl),    SymTblOff);
        m_rDatabase.Translate(Address(JmpRelTbl), JmpRelTblOff);
        m_rDatabase.Translate(Address(DynStr),    DynStrOff);
        m_rDatabase.Translate(Address(RelaTbl),   RelaTblOff);

        u8*   pReloc      = new u8[JmpRelSz];
        char* pDynSymStr  = new char[DynStrSz];
        u8*   pRelocA     = new u8[RelaSz];

        rBinStrm.Read(JmpRelTblOff, pReloc,     JmpRelSz);
        rBinStrm.Read(DynStrOff,    pDynSymStr, DynStrSz);
        rBinStrm.Read(RelaTblOff,   pRelocA,    RelaSz  );

        // XXX: At this time, it's unclear if we R_XXX_JMP_SLOT is always a Rel, Rela or both
        // We assume that it's Rela for now
        if (pReloc != NULL && JmpRelSz != 0x0)
        {
          if (PltRelType == DT_REL)
          {
            Medusa::Log() << "PLt relocs are Rel" << std::endl;
            typename TElfType::Rel* pRel = reinterpret_cast<typename TElfType::Rel*>(pReloc);
            for (u32 i = 0; i < JmpRelSz / sizeof(*pRel); ++pRel, ++i)
            {
              TElfType::EndianSwap(*pRel, m_Endianness);

              typename TElfType::Sym CurSym;
              u32     SymIdx    = pRel->r_info >> (sizeof(pRel->r_info) * 8 / 2);
              TOffset CurSymOff = SymTblOff + SymIdx * sizeof(CurSym);

              rBinStrm.Read(CurSymOff, &CurSym, sizeof(CurSym));

              if (pDynSymStr[CurSym.st_name] == '\0')
                continue;

              TElfType::EndianSwap(CurSym, m_Endianness);

              TOffset FuncOff;
              m_rDatabase.Translate(pRel->r_offset, FuncOff);

              typename TElfType::Addr FuncPlt;
              rBinStrm.Read(FuncOff, FuncPlt);

              Address FuncPltAddr(Address::FlatType, 0x0, FuncPlt, 0, sizeof(n) * 8);
              FuncPlt &= ~0xf;

              Medusa::Log()
                << "Symbol name: " << pDynSymStr + CurSym.st_name
                << ", address: " << std::hex << pRel->r_offset
                << ", plt: " << std::hex << FuncPlt
                << std::endl;

              Address FuncAddr(Address::FlatType, 0x0, static_cast<TOffset>(pRel->r_offset), 0, sizeof(n) * 8);
              std::string FuncName(pDynSymStr + CurSym.st_name);

              m_rDatabase.AddLabel(FuncAddr, Label(FuncName, Label::LabelData | Label::LabelImported));
              m_rDatabase.AddLabel(FuncPltAddr, Label(FuncName + "@plt", Label::LabelCode));
            }
          }
          else if (PltRelType == DT_RELA)
          {
            Medusa::Log() << "PLt relocs are Rela" << std::endl;
            typename TElfType::Rela* pRela = reinterpret_cast<typename TElfType::Rela*>(pReloc);
            for (u32 i = 0; i < JmpRelSz / sizeof(*pRela); ++pRela, ++i)
            {
              TElfType::EndianSwap(*pRela, m_Endianness);

              typename TElfType::Sym CurSym;
              u32     SymIdx    = pRela->r_info >> (sizeof(pRela->r_info) * 8 / 2);
              TOffset CurSymOff = SymTblOff + SymIdx * sizeof(CurSym);

              rBinStrm.Read(CurSymOff, &CurSym, sizeof(CurSym));

              if (pDynSymStr[CurSym.st_name] == '\0')
                continue;

              TElfType::EndianSwap(CurSym, m_Endianness);

              TOffset FuncOff;
              m_rDatabase.Translate(pRela->r_offset, FuncOff);

              typename TElfType::Addr FuncPlt;
              rBinStrm.Read(FuncOff, FuncPlt);

              FuncPlt &= ~0xf;

              Medusa::Log()
                << "Symbol name: " << pDynSymStr + CurSym.st_name
                << ", address: " << std::hex << pRela->r_offset
                << ", plt: " << std::hex << FuncPlt
                << std::endl;

              Address FuncAddr(static_cast<TOffset>(pRela->r_offset));
              std::string FuncName(pDynSymStr + CurSym.st_name);

              m_rDatabase.AddLabel(FuncAddr, Label(FuncName, Label::LabelData | Label::LabelImported));
              m_rDatabase.AddLabel(FuncPlt, Label(FuncName + "@plt", Label::LabelCode));
              //m_rDatabase.InsertMultiCell(FuncPlt, new Function);
            }
          } // if (PltRelType == DT_REL)
        } // if (pReloc != 0x0 && JmpRelSz != 0x0)

        if (pRelocA != NULL && RelaSz)
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

            Medusa::Log()
              << "Symbol name: " << pDynSymStr + CurSym.st_name
              << ", address: " << std::hex << pRela->r_offset
              << std::endl;

            Address SymAddr(static_cast<TOffset>(pRela->r_offset));
            std::string SymName(pDynSymStr + CurSym.st_name);

            // TODO: Use ELFXX_ST_TYPE instead
            if ((CurSym.st_info & 0xf) == STT_FUNC)
              m_rDatabase.AddLabel(SymAddr, Label(SymName, Label::LabelCode));
            else
              m_rDatabase.AddLabel(SymAddr, Label(SymName, Label::LabelData));
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
  Database&               m_rDatabase;
  EEndianness             m_Endianness;
  typedef ElfTraits<n>    TElfType;
  typename TElfType::Ehdr m_Ehdr;
  char*                   m_pShStrTbl;
};

#endif // !ELF_INTERPRETER_HPP