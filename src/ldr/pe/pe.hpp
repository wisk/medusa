#ifndef _PE_HEADER_
#define _PE_HEADER_

#include <medusa/types.hpp>
#include <medusa/endian.hpp>

MEDUSA_NAMESPACE_USE

#define PE_DOS_SIGNATURE               0x5A4D
#define PE_NT_SIGNATURE                0x00004550
#define PE_NT_OPTIONAL_HDR32_MAGIC     0x10b
#define PE_NT_OPTIONAL_HDR64_MAGIC     0x20b

#define PE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define PE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define PE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define PE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define PE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define PE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define PE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
  //      PE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
#define PE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define PE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define PE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define PE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define PE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define PE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define PE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define PE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor

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
#define PE_ORDINAL_FLAG64              0x8000000000000000ULL

struct PeDosHeader
{
  void Swap(EEndianness Endianness);
  enum { kMagic = PE_DOS_SIGNATURE };
  bool IsValid(void) const { return e_magic == kMagic; }
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
  void Swap(EEndianness Endianness);
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
  void Swap(EEndianness Endianness);
  u32 VirtualAddress;
  u32 Size;
};

struct PeOptionalHeader32
{
  void Swap(EEndianness Endianness);
  enum { kMagic = 0x10b };
  bool IsValid(void) const { return Magic == kMagic; }
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
  void Swap(EEndianness Endianness);
  enum { kSignature = PE_NT_SIGNATURE };
  bool IsValid(void) const { return Signature == kSignature; }
  u32                Signature;
  PeFileHeader       FileHeader;
  PeOptionalHeader32 OptionalHeader;
};

struct PeOptionalHeader64
{
  void Swap(EEndianness Endianness);
  enum { kMagic = 0x20b };
  bool IsValid(void) const { return Magic == kMagic; }
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
  void Swap(EEndianness Endianness);
  enum { kSignature = PE_NT_SIGNATURE };
  bool IsValid(void) const { return Signature == kSignature; }
  u32                Signature;
  PeFileHeader       FileHeader;
  PeOptionalHeader64 OptionalHeader;
};

struct PeSectionHeader
{
  void Swap(EEndianness Endianness);
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

struct PeImportDescriptor
{
  void Swap(EEndianness Endianness);
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

struct PeExportDirectory
{
  void Swap(EEndianness Endianness);
  u32 Characteristics;
  u32 TimeDateStamp;
  u16 MajorVersion;
  u16 MinorVersion;
  u32 Name;
  u32 Base;
  u32 NumberOfFunctions;
  u32 NumberOfNames;
  u32 AddressOfFunctions;
  u32 AddressOfNames;
  u32 AddressOfNameOrdinals;
};

struct PeThunkData32
{
  void Swap(EEndianness Endianness);
  bool IsOrdinal(void) const { return Ordinal & PE_ORDINAL_FLAG32 ? true : false; }
  u16  GetOrdinal(void) const { return Ordinal & 0xffff; }
  union
  {
    u32 ForwarderString;
    u32 Function;
    u32 Ordinal;
    u32 AddressOfData;
  };
};

struct PeThunkData64
{
  void Swap(EEndianness Endianness);
  bool IsOrdinal(void) const { return Ordinal & PE_ORDINAL_FLAG64 ? true : false; }
  u16  GetOrdinal(void) const { return Ordinal & 0xffff; }
  union
  {
    u64 ForwarderString;
    u64 Function;
    u64 Ordinal;
    u64 AddressOfData;
  };
};

struct PeImportByName
{
  void Swap(EEndianness Endianness);
  u16 Hint;
  u8 Name[1];
};

template<int bit> struct PeTraits {};

template<> struct PeTraits<32>
{
  typedef PeDosHeader        DosHeader;
  typedef PeFileHeader       FileHeader;
  typedef PeDataDirectory    DataDirectory;
  typedef PeOptionalHeader32 OptionalHeader;
  typedef PeNtHeaders32      NtHeaders;
  typedef PeSectionHeader    SectionHeader;
  typedef PeImportDescriptor ImportDescriptor;
  typedef PeThunkData32      ThunkData;
  typedef PeImportByName     ImportByName;
  typedef PeExportDirectory  ExportDirectory;
};

template<> struct PeTraits<64>
{
  typedef PeDosHeader         DosHeader;
  typedef PeFileHeader        FileHeader;
  typedef PeDataDirectory     DataDirectory;
  typedef PeOptionalHeader64  OptionalHeader;
  typedef PeNtHeaders64       NtHeaders;
  typedef PeSectionHeader     SectionHeader;
  typedef PeImportDescriptor  ImportDescriptor;
  typedef PeThunkData64       ThunkData;
  typedef PeImportByName      ImportByName;
  typedef PeExportDirectory   ExportDirectory;
};

#endif
