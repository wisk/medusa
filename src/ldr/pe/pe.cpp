#include "pe.hpp"

#define DEF_STRUCT_SWAP(struct_name, do_swap)\
  void Swap##struct_name (struct_name & r##struct_name, EEndianness Endianness)\
  {\
    if (!TestEndian(Endianness)) return;\
    do_swap\
  }

DEF_STRUCT_SWAP(PeDosHeader,
  ::EndianSwap(rPeDosHeader.e_magic);
  ::EndianSwap(rPeDosHeader.e_cblp);
  ::EndianSwap(rPeDosHeader.e_cp);
  ::EndianSwap(rPeDosHeader.e_crlc);
  ::EndianSwap(rPeDosHeader.e_cparhdr);
  ::EndianSwap(rPeDosHeader.e_minalloc);
  ::EndianSwap(rPeDosHeader.e_maxalloc);
  ::EndianSwap(rPeDosHeader.e_ss);
  ::EndianSwap(rPeDosHeader.e_sp);
  ::EndianSwap(rPeDosHeader.e_csum);
  ::EndianSwap(rPeDosHeader.e_ip);
  ::EndianSwap(rPeDosHeader.e_cs);
  ::EndianSwap(rPeDosHeader.e_lfarlc);
  ::EndianSwap(rPeDosHeader.e_ovno);
  ::EndianSwap(rPeDosHeader.e_res[4]);
  ::EndianSwap(rPeDosHeader.e_oemid);
  ::EndianSwap(rPeDosHeader.e_oeminfo);
  ::EndianSwap(rPeDosHeader.e_res2[10]);
  ::EndianSwap(rPeDosHeader.e_lfanew);
)

DEF_STRUCT_SWAP(PeFileHeader,
  ::EndianSwap(rPeFileHeader.Machine);
  ::EndianSwap(rPeFileHeader.NumberOfSections);
  ::EndianSwap(rPeFileHeader.TimeDateStamp);
  ::EndianSwap(rPeFileHeader.PointerToSymbolTable);
  ::EndianSwap(rPeFileHeader.NumberOfSymbols);
  ::EndianSwap(rPeFileHeader.SizeOfOptionalHeader);
  ::EndianSwap(rPeFileHeader.Characteristics);
)

DEF_STRUCT_SWAP(PeDataDirectory,
  ::EndianSwap(rPeDataDirectory.VirtualAddress);
  ::EndianSwap(rPeDataDirectory.Size);
)

DEF_STRUCT_SWAP(PeOptionalHeader32,
  ::EndianSwap(rPeOptionalHeader32.Magic);
  ::EndianSwap(rPeOptionalHeader32.MajorLinkerVersion);
  ::EndianSwap(rPeOptionalHeader32.MinorLinkerVersion);
  ::EndianSwap(rPeOptionalHeader32.SizeOfCode);
  ::EndianSwap(rPeOptionalHeader32.SizeOfInitializedData);
  ::EndianSwap(rPeOptionalHeader32.SizeOfUninitializedData);
  ::EndianSwap(rPeOptionalHeader32.AddressOfEntryPoint);
  ::EndianSwap(rPeOptionalHeader32.BaseOfCode);
  ::EndianSwap(rPeOptionalHeader32.BaseOfData);
  ::EndianSwap(rPeOptionalHeader32.ImageBase);
  ::EndianSwap(rPeOptionalHeader32.SectionAlignment);
  ::EndianSwap(rPeOptionalHeader32.FileAlignment);
  ::EndianSwap(rPeOptionalHeader32.MajorOperatingSystemVersion);
  ::EndianSwap(rPeOptionalHeader32.MinorOperatingSystemVersion);
  ::EndianSwap(rPeOptionalHeader32.MajorImageVersion);
  ::EndianSwap(rPeOptionalHeader32.MinorImageVersion);
  ::EndianSwap(rPeOptionalHeader32.MajorSubsystemVersion);
  ::EndianSwap(rPeOptionalHeader32.MinorSubsystemVersion);
  ::EndianSwap(rPeOptionalHeader32.Win32VersionValue);
  ::EndianSwap(rPeOptionalHeader32.SizeOfImage);
  ::EndianSwap(rPeOptionalHeader32.SizeOfHeaders);
  ::EndianSwap(rPeOptionalHeader32.CheckSum);
  ::EndianSwap(rPeOptionalHeader32.Subsystem);
  ::EndianSwap(rPeOptionalHeader32.DllCharacteristics);
  ::EndianSwap(rPeOptionalHeader32.SizeOfStackReserve);
  ::EndianSwap(rPeOptionalHeader32.SizeOfStackCommit);
  ::EndianSwap(rPeOptionalHeader32.SizeOfHeapReserve);
  ::EndianSwap(rPeOptionalHeader32.SizeOfHeapCommit);
  ::EndianSwap(rPeOptionalHeader32.LoaderFlags);
  ::EndianSwap(rPeOptionalHeader32.NumberOfRvaAndSizes);
  for (u32 i = 0; i < PE_NUMBEROF_DIRECTORY_ENTRIES; ++i)
    SwapPeDataDirectory(rPeOptionalHeader32.DataDirectory[i], Endianness);
)

DEF_STRUCT_SWAP(PeNtHeaders32,
  ::EndianSwap(rPeNtHeaders32.Signature);
  ::EndianSwap(rPeNtHeaders32.FileHeader);
  SwapPeOptionalHeader32(rPeNtHeaders32.OptionalHeader, Endianness);
)

DEF_STRUCT_SWAP(PeOptionalHeader64,
  ::EndianSwap(rPeOptionalHeader64.Magic);
  ::EndianSwap(rPeOptionalHeader64.MajorLinkerVersion);
  ::EndianSwap(rPeOptionalHeader64.MinorLinkerVersion);
  ::EndianSwap(rPeOptionalHeader64.SizeOfCode);
  ::EndianSwap(rPeOptionalHeader64.SizeOfInitializedData);
  ::EndianSwap(rPeOptionalHeader64.SizeOfUninitializedData);
  ::EndianSwap(rPeOptionalHeader64.AddressOfEntryPoint);
  ::EndianSwap(rPeOptionalHeader64.BaseOfCode);
  ::EndianSwap(rPeOptionalHeader64.ImageBase);
  ::EndianSwap(rPeOptionalHeader64.SectionAlignment);
  ::EndianSwap(rPeOptionalHeader64.FileAlignment);
  ::EndianSwap(rPeOptionalHeader64.MajorOperatingSystemVersion);
  ::EndianSwap(rPeOptionalHeader64.MinorOperatingSystemVersion);
  ::EndianSwap(rPeOptionalHeader64.MajorImageVersion);
  ::EndianSwap(rPeOptionalHeader64.MinorImageVersion);
  ::EndianSwap(rPeOptionalHeader64.MajorSubsystemVersion);
  ::EndianSwap(rPeOptionalHeader64.MinorSubsystemVersion);
  ::EndianSwap(rPeOptionalHeader64.Win32VersionValue);
  ::EndianSwap(rPeOptionalHeader64.SizeOfImage);
  ::EndianSwap(rPeOptionalHeader64.SizeOfHeaders);
  ::EndianSwap(rPeOptionalHeader64.CheckSum);
  ::EndianSwap(rPeOptionalHeader64.Subsystem);
  ::EndianSwap(rPeOptionalHeader64.DllCharacteristics);
  ::EndianSwap(rPeOptionalHeader64.SizeOfStackReserve);
  ::EndianSwap(rPeOptionalHeader64.SizeOfStackCommit);
  ::EndianSwap(rPeOptionalHeader64.SizeOfHeapReserve);
  ::EndianSwap(rPeOptionalHeader64.SizeOfHeapCommit);
  ::EndianSwap(rPeOptionalHeader64.LoaderFlags);
  ::EndianSwap(rPeOptionalHeader64.NumberOfRvaAndSizes);
  for (u64 i = 0; i < PE_NUMBEROF_DIRECTORY_ENTRIES; ++i)
    SwapPeDataDirectory(rPeOptionalHeader64.DataDirectory[i], Endianness);
)

DEF_STRUCT_SWAP(PeNtHeaders64,
  ::EndianSwap(rPeNtHeaders64.Signature);
  ::EndianSwap(rPeNtHeaders64.FileHeader);
  SwapPeOptionalHeader64(rPeNtHeaders64.OptionalHeader, Endianness);
)

DEF_STRUCT_SWAP(PeSectionHeader,
  //Name[PE_SIZEOF_SHORT_NAME];
  ::EndianSwap(rPeSectionHeader.Misc.VirtualSize);
  ::EndianSwap(rPeSectionHeader.VirtualAddress);
  ::EndianSwap(rPeSectionHeader.SizeOfRawData);
  ::EndianSwap(rPeSectionHeader.PointerToRawData);
  ::EndianSwap(rPeSectionHeader.PointerToRelocations);
  ::EndianSwap(rPeSectionHeader.PointerToLinenumbers);
  ::EndianSwap(rPeSectionHeader.NumberOfRelocations);
  ::EndianSwap(rPeSectionHeader.NumberOfLinenumbers);
  ::EndianSwap(rPeSectionHeader.Characteristics);
)

DEF_STRUCT_SWAP(ImageImportDescriptor,
  ::EndianSwap(rImageImportDescriptor.OriginalFirstThunk);
  ::EndianSwap(rImageImportDescriptor.TimeDateStamp);
  ::EndianSwap(rImageImportDescriptor.ForwarderChain);
  ::EndianSwap(rImageImportDescriptor.Name);
  ::EndianSwap(rImageImportDescriptor.FirstThunk);
)

/*DEF_STRUCT_SWAP(ImageThunkData32,
  ::EndianSwap(rImageThunkData32.Function);
  )*/

/*DEF_STRUCT_SWAP(ImageThunkData64,
  ::EndianSwap(rImageThunkData64.Function);
  )*/

DEF_STRUCT_SWAP(ImageImportByName,
  ::EndianSwap(rImageImportByName.Hint);
  //Name[1];
)

#undef DEF_STRUCT_SWAP
