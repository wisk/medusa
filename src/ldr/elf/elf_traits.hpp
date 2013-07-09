#ifndef ELF_TRAITS_HPP
#define ELF_TRAITS_HPP

#include "elf.h"
#include <medusa/endian.hpp>

template<int N> struct ElfTraits
{
};

template<> struct ElfTraits<32>
{
  typedef Elf32_Half    Half;
  typedef Elf32_Word    Word;
  typedef Elf32_Sword   Sword;
  typedef Elf32_Xword   Xword;
  typedef Elf32_Sxword  Sxword;
  typedef Elf32_Addr    Addr;
  typedef Elf32_Off     Off;
  typedef Elf32_Section Section;
  typedef Elf32_Versym  Versym;

  typedef Elf32_Ehdr    Ehdr;
  typedef Elf32_Shdr    Shdr;
  typedef Elf32_Phdr    Phdr;
  typedef Elf32_Sym     Sym;
  typedef Elf32_Rel     Rel;
  typedef Elf32_Rela    Rela;
  typedef Elf32_Dyn     Dyn;

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
  typedef Elf64_Half    Half;
  typedef Elf64_Word    Word;
  typedef Elf64_Sword   Sword;
  typedef Elf64_Xword   Xword;
  typedef Elf64_Sxword  Sxword;
  typedef Elf64_Addr    Addr;
  typedef Elf64_Off     Off;
  typedef Elf64_Section Section;
  typedef Elf64_Versym  Versym;

  typedef Elf64_Ehdr    Ehdr;
  typedef Elf64_Shdr    Shdr;
  typedef Elf64_Phdr    Phdr;
  typedef Elf64_Sym     Sym;
  typedef Elf64_Rel     Rel;
  typedef Elf64_Rela    Rela;
  typedef Elf64_Dyn     Dyn;

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

#endif // !ELF_TRAITS_HPP
