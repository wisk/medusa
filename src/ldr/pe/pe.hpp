#ifndef _PE_HEADER_
#define _PE_HEADER_

#include <medusa/types.hpp>
#include <medusa/endian.hpp>

MEDUSA_NAMESPACE_USE

#define PE_DOS_SIGNATURE               0x5A4D
#define PE_NT_SIGNATURE                0x00004550
#define PE_NT_OPTIONAL_HDR32_MAGIC     0x10b
#define PE_NT_OPTIONAL_HDR64_MAGIC     0x20b

#define PE_NUMBEROF_DIRECTORY_ENTRIES  0x10
#define PE_SIZEOF_SHORT_NAME           0x8

#define PE_FILE_MACHINE_UNKNOWN        0x0000
#define PE_FILE_MACHINE_AM33           0x01d3 /* Matsushita AM33 */
#define PE_FILE_MACHINE_AMD64          0x8664 /* x64 */
#define PE_FILE_MACHINE_ARM            0x01c0 /* ARM little endian */
#define PE_FILE_MACHINE_EBC            0x0ebc /* EFI u8 code */
#define PE_FILE_MACHINE_I386           0x014c /* Intel 386 or later processors and compatible processors */
#define PE_FILE_MACHINE_IA64           0x0200 /* Intel Itanium processor family */
#define PE_FILE_MACHINE_M32R           0x9041 /* Mitsubishi M32R little endian */
#define PE_FILE_MACHINE_MIPS16         0x0266 /* MIPS16 */
#define PE_FILE_MACHINE_MIPSFPU        0x0366 /* MIPS with FPU */
#define PE_FILE_MACHINE_MIPSFPU16      0x0466 /* MIPS16 with FPU */
#define PE_FILE_MACHINE_POWERPC        0x01f0 /* Power PC little endian */
#define PE_FILE_MACHINE_POWERPCFP      0x01f1 /* Power PC with floating point support */
#define PE_FILE_MACHINE_R4000          0x0166 /* MIPS little endian */
#define PE_FILE_MACHINE_SH3            0x01a2 /* Hitachi SH3 */
#define PE_FILE_MACHINE_SH3DSP         0x01a3 /* Hitachi SH3 DSP */
#define PE_FILE_MACHINE_SH4            0x01a6 /* Hitachi SH4 */
#define PE_FILE_MACHINE_SH5            0x01a8 /* Hitachi SH5 */
#define PE_FILE_MACHINE_THUMB          0x01c2 /* Thumb */
#define PE_FILE_MACHINE_WCEMIPSV2      0x0169 /* MIPS little-endian WCE v2 */

#define PE_SCN_CNT_CODE                0x20
#define PE_SCN_CNT_INITIALIZED_DATA    0x40
#define PE_SCN_CNT_UNINITIALIZED_DATA  0x80
#define PE_SCN_MEM_EXECUTE             0x20000000
#define PE_SCN_MEM_READ                0x40000000
#define PE_SCN_MEM_WRITE               0x80000000

#define PE_ORDINAL_FLAG32              0x80000000
#define PE_ORDINAL_FLAG64              (((u64)0x80000000 << 32) | 0x00000000)


struct PeDosHeader
{
  u16 e_magic;
  u16 e_cblp;
  u16 e_cp;
  u16 e_crlc;
  u16 e_cparhdr;
  u16 e_minalloc;
  u16 e_maxalloc;
  u16 e_ss;
  u16 e_sp;
  u16 e_csum;
  u16 e_ip;
  u16 e_cs;
  u16 e_lfarlc;
  u16 e_ovno;
  u16 e_res[4];
  u16 e_oemid;
  u16 e_oeminfo;
  u16 e_res2[10];
  u32 e_lfanew;
};

struct PeFileHeader
{
  u16 Machine;
  u16 NumberOfSections;
  u32 TimeDateStamp;
  u32 PointerToSymbolTable;
  u32 NumberOfSymbols;
  u16 SizeOfOptionalHeader;
  u16 Characteristics;
};

struct PeDataDirectory
{
  u32 VirtualAddress;
  u32 Size;
};

struct PeOptionalHeader32
{
  u16             Magic;
  u8              MajorLinkerVersion;
  u8              MinorLinkerVersion;
  u32             SizeOfCode;
  u32             SizeOfInitializedData;
  u32             SizeOfUninitializedData;
  u32             AddressOfEntryPoint;
  u32             BaseOfCode;
  u32             BaseOfData;
  u32             ImageBase;
  u32             SectionAlignment;
  u32             FileAlignment;
  u16             MajorOperatingSystemVersion;
  u16             MinorOperatingSystemVersion;
  u16             MajorImageVersion;
  u16             MinorImageVersion;
  u16             MajorSubsystemVersion;
  u16             MinorSubsystemVersion;
  u32             Win32VersionValue;
  u32             SizeOfImage;
  u32             SizeOfHeaders;
  u32             CheckSum;
  u16             Subsystem;
  u16             DllCharacteristics;
  u32             SizeOfStackReserve;
  u32             SizeOfStackCommit;
  u32             SizeOfHeapReserve;
  u32             SizeOfHeapCommit;
  u32             LoaderFlags;
  u32             NumberOfRvaAndSizes;
  PeDataDirectory DataDirectory[PE_NUMBEROF_DIRECTORY_ENTRIES];
};

struct PeNtHeaders32
{
  u32                Signature;
  PeFileHeader       FileHeader;
  PeOptionalHeader32 OptionalHeader;
};

struct PeOptionalHeader64
{
  u16             Magic;
  u8              MajorLinkerVersion;
  u8              MinorLinkerVersion;
  u32             SizeOfCode;
  u32             SizeOfInitializedData;
  u32             SizeOfUninitializedData;
  u32             AddressOfEntryPoint;
  u32             BaseOfCode;
  u64             ImageBase;
  u32             SectionAlignment;
  u32             FileAlignment;
  u16             MajorOperatingSystemVersion;
  u16             MinorOperatingSystemVersion;
  u16             MajorImageVersion;
  u16             MinorImageVersion;
  u16             MajorSubsystemVersion;
  u16             MinorSubsystemVersion;
  u32             Win32VersionValue;
  u32             SizeOfImage;
  u32             SizeOfHeaders;
  u32             CheckSum;
  u16             Subsystem;
  u16             DllCharacteristics;
  u64             SizeOfStackReserve;
  u64             SizeOfStackCommit;
  u64             SizeOfHeapReserve;
  u64             SizeOfHeapCommit;
  u32             LoaderFlags;
  u32             NumberOfRvaAndSizes;
  PeDataDirectory DataDirectory[PE_NUMBEROF_DIRECTORY_ENTRIES];
};

struct PeNtHeaders64
{
  u32                Signature;
  PeFileHeader       FileHeader;
  PeOptionalHeader64 OptionalHeader;
};

struct PeSectionHeader
{
  u8  Name[PE_SIZEOF_SHORT_NAME];
  union
  {
      u32 PhysicalAddress;
      u32 VirtualSize;
  }   Misc;
  u32 VirtualAddress;
  u32 SizeOfRawData;
  u32 PointerToRawData;
  u32 PointerToRelocations;
  u32 PointerToLinenumbers;
  u16 NumberOfRelocations;
  u16 NumberOfLinenumbers;
  u32 Characteristics;
};

struct ImageImportDescriptor
{
  union
  {
    u32 Characteristics;
    u32 OriginalFirstThunk; // Import Name Table offset
  };
  u32 TimeDateStamp;        // 
  u32 ForwarderChain;       // 
  u32 Name;                 // Dll Name offset
  u32 FirstThunk;           // Import Address Table offset
};

template<typename DataType> struct ImageThunkData
{
  union
  {
    DataType ForwarderString;
    DataType Function;
    DataType Ordinal;
    DataType AddressOfData;
  };
};

/*struct ImageThunkData64
{
  union
  {
    u64 ForwarderString;
    u64 Function;
    u64 Ordinal;
    u64 AddressOfData;
  };
  };*/

struct ImageImportByName
{
  u16 Hint;
  u8 Name[1];
};

#define DECL_STRUCT_SWAP(struct_name) void Swap##struct_name (struct_name & r##struct_name, EEndianness Endianness)

DECL_STRUCT_SWAP(PeDosHeader);

DECL_STRUCT_SWAP(PeFileHeader);
DECL_STRUCT_SWAP(PeDataDirectory);

DECL_STRUCT_SWAP(PeOptionalHeader32);
DECL_STRUCT_SWAP(PeNtHeaders32);

DECL_STRUCT_SWAP(PeOptionalHeader64);
DECL_STRUCT_SWAP(PeNtHeaders64);

DECL_STRUCT_SWAP(PeSectionHeader);

DECL_STRUCT_SWAP(ImageImportDescriptor);

// TODO SWAP ImageThunkData<>
//DECL_STRUCT_SWAP(ImageThunkData32);
//DECL_STRUCT_SWAP(ImageThunkData64);

DECL_STRUCT_SWAP(ImageImportByName);

#undef DECL_STRUCT_SWAP

#endif
