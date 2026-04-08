#ifndef LDR_ELF_TRAITS_HPP
#define LDR_ELF_TRAITS_HPP

#include "elf.h"
#include <medusa/endian.hpp>

template<int N> struct ElfTraits
{
};

template<> struct ElfTraits<32>
{
  using Half    = Elf32_Half;
  using Word    = Elf32_Word;
  using Sword   = Elf32_Sword;
  using Xword   = Elf32_Xword;
  using Sxword  = Elf32_Sxword;
  using Addr    = Elf32_Addr;
  using Off     = Elf32_Off;
  using Section = Elf32_Section;
  using Versym  = Elf32_Versym;

  using Ehdr = Elf32_Ehdr;
  using Shdr = Elf32_Shdr;
  using Phdr = Elf32_Phdr;
  using Sym  = Elf32_Sym;
  using Rel  = Elf32_Rel;
  using Rela = Elf32_Rela;
  using Dyn  = Elf32_Dyn;

  static void EndianSwap(Ehdr& rEhdr, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rEhdr.e_type);
    ::EndianSwap(rEhdr.e_machine);
    ::EndianSwap(rEhdr.e_version);
    ::EndianSwap(rEhdr.e_entry);
    ::EndianSwap(rEhdr.e_phoff);
    ::EndianSwap(rEhdr.e_shoff);
    ::EndianSwap(rEhdr.e_flags);
    ::EndianSwap(rEhdr.e_ehsize);
    ::EndianSwap(rEhdr.e_phentsize);
    ::EndianSwap(rEhdr.e_phnum);
    ::EndianSwap(rEhdr.e_shentsize);
    ::EndianSwap(rEhdr.e_shnum);
    ::EndianSwap(rEhdr.e_shstrndx);
  }

  static void EndianSwap(Shdr& rShdr, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rShdr.sh_name);
    ::EndianSwap(rShdr.sh_type);
    ::EndianSwap(rShdr.sh_flags);
    ::EndianSwap(rShdr.sh_addr);
    ::EndianSwap(rShdr.sh_offset);
    ::EndianSwap(rShdr.sh_size);
    ::EndianSwap(rShdr.sh_link);
    ::EndianSwap(rShdr.sh_info);
    ::EndianSwap(rShdr.sh_addralign);
    ::EndianSwap(rShdr.sh_entsize);
  }

  static void EndianSwap(Phdr& rPhdr, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rPhdr.p_type);
    ::EndianSwap(rPhdr.p_flags);
    ::EndianSwap(rPhdr.p_offset);
    ::EndianSwap(rPhdr.p_vaddr);
    ::EndianSwap(rPhdr.p_paddr);
    ::EndianSwap(rPhdr.p_filesz);
    ::EndianSwap(rPhdr.p_memsz);
    ::EndianSwap(rPhdr.p_align);
  }

  static void EndianSwap(Sym& rSym, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rSym.st_name);
    ::EndianSwap(rSym.st_value);
    ::EndianSwap(rSym.st_size);
    ::EndianSwap(rSym.st_info);
    ::EndianSwap(rSym.st_other);
    ::EndianSwap(rSym.st_shndx);
  }

  static void EndianSwap(Rel& rRel, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rRel.r_offset);
    ::EndianSwap(rRel.r_info);
  }

  static void EndianSwap(Rela& rRela, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rRela.r_offset);
    ::EndianSwap(rRela.r_info);
    ::EndianSwap(rRela.r_addend);
  }

  static void EndianSwap(Dyn& rDyn, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rDyn.d_tag);
    ::EndianSwap(rDyn.d_un.d_val);
  }
};

template<> struct ElfTraits<64>
{
  using Half    = Elf64_Half;
  using Word    = Elf64_Word;
  using Sword   = Elf64_Sword;
  using Xword   = Elf64_Xword;
  using Sxword  = Elf64_Sxword;
  using Addr    = Elf64_Addr;
  using Off     = Elf64_Off;
  using Section = Elf64_Section;
  using Versym  = Elf64_Versym;

  using Ehdr = Elf64_Ehdr;
  using Shdr = Elf64_Shdr;
  using Phdr = Elf64_Phdr;
  using Sym  = Elf64_Sym;
  using Rel  = Elf64_Rel;
  using Rela = Elf64_Rela;
  using Dyn  = Elf64_Dyn;

  static void EndianSwap(Ehdr& rEhdr, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rEhdr.e_type);
    ::EndianSwap(rEhdr.e_machine);
    ::EndianSwap(rEhdr.e_version);
    ::EndianSwap(rEhdr.e_entry);
    ::EndianSwap(rEhdr.e_phoff);
    ::EndianSwap(rEhdr.e_shoff);
    ::EndianSwap(rEhdr.e_flags);
    ::EndianSwap(rEhdr.e_ehsize);
    ::EndianSwap(rEhdr.e_phentsize);
    ::EndianSwap(rEhdr.e_phnum);
    ::EndianSwap(rEhdr.e_shentsize);
    ::EndianSwap(rEhdr.e_shnum);
    ::EndianSwap(rEhdr.e_shstrndx);
  }

  static void EndianSwap(Shdr& rShdr, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rShdr.sh_name);
    ::EndianSwap(rShdr.sh_type);
    ::EndianSwap(rShdr.sh_flags);
    ::EndianSwap(rShdr.sh_addr);
    ::EndianSwap(rShdr.sh_offset);
    ::EndianSwap(rShdr.sh_size);
    ::EndianSwap(rShdr.sh_link);
    ::EndianSwap(rShdr.sh_info);
    ::EndianSwap(rShdr.sh_addralign);
    ::EndianSwap(rShdr.sh_entsize);
  }

  static void EndianSwap(Phdr& rPhdr, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rPhdr.p_type);
    ::EndianSwap(rPhdr.p_flags);
    ::EndianSwap(rPhdr.p_offset);
    ::EndianSwap(rPhdr.p_vaddr);
    ::EndianSwap(rPhdr.p_paddr);
    ::EndianSwap(rPhdr.p_filesz);
    ::EndianSwap(rPhdr.p_memsz);
    ::EndianSwap(rPhdr.p_align);
  }

  static void EndianSwap(Sym& rSym, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rSym.st_name);
    ::EndianSwap(rSym.st_info);
    ::EndianSwap(rSym.st_other);
    ::EndianSwap(rSym.st_shndx);
    ::EndianSwap(rSym.st_value);
    ::EndianSwap(rSym.st_size);
  }

  static void EndianSwap(Rel& rRel, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rRel.r_offset);
    ::EndianSwap(rRel.r_info);
  }

  static void EndianSwap(Rela& rRela, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rRela.r_offset);
    ::EndianSwap(rRela.r_info);
    ::EndianSwap(rRela.r_addend);
  }

  static void EndianSwap(Dyn& rDyn, EEndianness Endianness)
  {
    if (!TestEndian(Endianness))
      return;

    ::EndianSwap(rDyn.d_tag);
    ::EndianSwap(rDyn.d_un.d_val);
  }
};

#endif // !LDR_ELF_TRAITS_HPP
