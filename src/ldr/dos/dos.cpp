#include "dos.hpp"


#define DEF_STRUCT_SWAP(struct_name, do_swap)\
  void struct_name::Swap(EEndianness Endianness)\
  {\
    if (!::TestEndian(Endianness)) return;\
    do_swap\
  }

DEF_STRUCT_SWAP(DosHeader,
  ::EndianSwap(e_magic);
  ::EndianSwap(e_cblp);
  ::EndianSwap(e_cp);
  ::EndianSwap(e_crlc);
  ::EndianSwap(e_cparhdr);
  ::EndianSwap(e_minalloc);
  ::EndianSwap(e_maxalloc);
  ::EndianSwap(e_ss);
  ::EndianSwap(e_sp);
  ::EndianSwap(e_csum);
  ::EndianSwap(e_ip);
  ::EndianSwap(e_cs);
  ::EndianSwap(e_lfarlc);
  ::EndianSwap(e_ovno);
)
