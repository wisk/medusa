#include "pe.hpp"

#define DEF_STRUCT_SWAP(struct_name, do_swap)\
  void struct_name::Swap(EEndianness Endianness)\
  {\
    if (!::TestEndian(Endianness)) return;\
    do_swap\
  }

DEF_STRUCT_SWAP(PeDosHeader,
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
  ::EndianSwap(e_res[4]);
  ::EndianSwap(e_oemid);
  ::EndianSwap(e_oeminfo);
  ::EndianSwap(e_res2[10]);
  ::EndianSwap(e_lfanew);
)

DEF_STRUCT_SWAP(PeFileHeader,
  ::EndianSwap(Machine);
  ::EndianSwap(NumberOfSections);
  ::EndianSwap(TimeDateStamp);
  ::EndianSwap(PointerToSymbolTable);
  ::EndianSwap(NumberOfSymbols);
  ::EndianSwap(SizeOfOptionalHeader);
  ::EndianSwap(Characteristics);
)

DEF_STRUCT_SWAP(PeDataDirectory,
  ::EndianSwap(VirtualAddress);
  ::EndianSwap(Size);
)

DEF_STRUCT_SWAP(PeOptionalHeader32,
  ::EndianSwap(Magic);
  ::EndianSwap(MajorLinkerVersion);
  ::EndianSwap(MinorLinkerVersion);
  ::EndianSwap(SizeOfCode);
  ::EndianSwap(SizeOfInitializedData);
  ::EndianSwap(SizeOfUninitializedData);
  ::EndianSwap(AddressOfEntryPoint);
  ::EndianSwap(BaseOfCode);
  ::EndianSwap(BaseOfData);
  ::EndianSwap(ImageBase);
  ::EndianSwap(SectionAlignment);
  ::EndianSwap(FileAlignment);
  ::EndianSwap(MajorOperatingSystemVersion);
  ::EndianSwap(MinorOperatingSystemVersion);
  ::EndianSwap(MajorImageVersion);
  ::EndianSwap(MinorImageVersion);
  ::EndianSwap(MajorSubsystemVersion);
  ::EndianSwap(MinorSubsystemVersion);
  ::EndianSwap(Win32VersionValue);
  ::EndianSwap(SizeOfImage);
  ::EndianSwap(SizeOfHeaders);
  ::EndianSwap(CheckSum);
  ::EndianSwap(Subsystem);
  ::EndianSwap(DllCharacteristics);
  ::EndianSwap(SizeOfStackReserve);
  ::EndianSwap(SizeOfStackCommit);
  ::EndianSwap(SizeOfHeapReserve);
  ::EndianSwap(SizeOfHeapCommit);
  ::EndianSwap(LoaderFlags);
  ::EndianSwap(NumberOfRvaAndSizes);
  for (u32 i = 0; i < PE_NUMBEROF_DIRECTORY_ENTRIES; ++i)
    DataDirectory[i].Swap(Endianness);
)

DEF_STRUCT_SWAP(PeNtHeaders32,
  ::EndianSwap(Signature);
  FileHeader.Swap(Endianness);
  OptionalHeader.Swap(Endianness);
)

DEF_STRUCT_SWAP(PeOptionalHeader64,
  ::EndianSwap(Magic);
  ::EndianSwap(MajorLinkerVersion);
  ::EndianSwap(MinorLinkerVersion);
  ::EndianSwap(SizeOfCode);
  ::EndianSwap(SizeOfInitializedData);
  ::EndianSwap(SizeOfUninitializedData);
  ::EndianSwap(AddressOfEntryPoint);
  ::EndianSwap(BaseOfCode);
  ::EndianSwap(ImageBase);
  ::EndianSwap(SectionAlignment);
  ::EndianSwap(FileAlignment);
  ::EndianSwap(MajorOperatingSystemVersion);
  ::EndianSwap(MinorOperatingSystemVersion);
  ::EndianSwap(MajorImageVersion);
  ::EndianSwap(MinorImageVersion);
  ::EndianSwap(MajorSubsystemVersion);
  ::EndianSwap(MinorSubsystemVersion);
  ::EndianSwap(Win32VersionValue);
  ::EndianSwap(SizeOfImage);
  ::EndianSwap(SizeOfHeaders);
  ::EndianSwap(CheckSum);
  ::EndianSwap(Subsystem);
  ::EndianSwap(DllCharacteristics);
  ::EndianSwap(SizeOfStackReserve);
  ::EndianSwap(SizeOfStackCommit);
  ::EndianSwap(SizeOfHeapReserve);
  ::EndianSwap(SizeOfHeapCommit);
  ::EndianSwap(LoaderFlags);
  ::EndianSwap(NumberOfRvaAndSizes);
  for (u64 i = 0; i < PE_NUMBEROF_DIRECTORY_ENTRIES; ++i)
    DataDirectory[i].Swap(Endianness);
)

DEF_STRUCT_SWAP(PeNtHeaders64,
  ::EndianSwap(Signature);
  FileHeader.Swap(Endianness);
  OptionalHeader.Swap(Endianness);
)

DEF_STRUCT_SWAP(PeSectionHeader,
  //Name[PE_SIZEOF_SHORT_NAME];
  ::EndianSwap(Misc.VirtualSize);
  ::EndianSwap(VirtualAddress);
  ::EndianSwap(SizeOfRawData);
  ::EndianSwap(PointerToRawData);
  ::EndianSwap(PointerToRelocations);
  ::EndianSwap(PointerToLinenumbers);
  ::EndianSwap(NumberOfRelocations);
  ::EndianSwap(NumberOfLinenumbers);
  ::EndianSwap(Characteristics);
)

DEF_STRUCT_SWAP(PeImportDescriptor,
  ::EndianSwap(OriginalFirstThunk);
  ::EndianSwap(TimeDateStamp);
  ::EndianSwap(ForwarderChain);
  ::EndianSwap(Name);
  ::EndianSwap(FirstThunk);
)

DEF_STRUCT_SWAP(PeThunkData32,
  ::EndianSwap(Function);
)

DEF_STRUCT_SWAP(PeThunkData64,
  ::EndianSwap(Function);
)

DEF_STRUCT_SWAP(PeImportByName,
  ::EndianSwap(Hint);
  //Name[1];
)

#undef DEF_STRUCT_SWAP
