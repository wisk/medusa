#define BOOST_TEST_MODULE TestCore
#include <boost/test/unit_test.hpp>

#include <medusa/medusa.hpp>
#include <medusa/detail.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(core_test_suite)

BOOST_AUTO_TEST_CASE(core_structure_test_case)
{
  BOOST_MESSAGE("Testing structure");

  using namespace medusa;

  auto BYTE = std::make_shared<TypeDetail>("BYTE", TypeDetail::IntegerType, 8);
  auto WORD = std::make_shared<TypeDetail>("WORD", TypeDetail::IntegerType, 16);
  auto LONG = std::make_shared<TypeDetail>("LONG", TypeDetail::IntegerType, 32);
  auto DWORD = std::make_shared<TypeDetail>("DWORD", TypeDetail::IntegerType, 32);

  StructureDetail _IMAGE_FILE_HEADER("_IMAGE_FILE_HEADER");
  _IMAGE_FILE_HEADER
    .AddField(WORD, "Machine")
    .AddField(WORD, "NumberOfSections")
    .AddField(DWORD, "TimeDateStamp")
    .AddField(DWORD, "PointerToSymbolTable")
    .AddField(DWORD, "NumberOfSymbols")
    .AddField(WORD, "SizeOfOptionalHeader")
    .AddField(WORD, "Characteristics")
    ;

  StructureDetail _IMAGE_DATA_DIRECTORY ("_IMAGE_DATA_DIRECTORY");
  _IMAGE_DATA_DIRECTORY 
    .AddField(DWORD, "VirtualAddress")
    .AddField(DWORD, "Size")
    ;

  StructureDetail _IMAGE_OPTIONAL_HEADER("_IMAGE_OPTIONAL_HEADER");
  _IMAGE_OPTIONAL_HEADER
    .AddField(WORD, "Magic")
    .AddField(BYTE, "MajorLinkerVersion")
    .AddField(BYTE, "MinorLinkerVersion")
    .AddField(DWORD, "SizeOfCode")
    .AddField(DWORD, "SizeOfInitializedData")
    .AddField(DWORD, "SizeOfUninitializedData")
    .AddField(DWORD, "AddressOfEntryPoint")
    .AddField(DWORD, "BaseOfCode")
    .AddField(DWORD, "BaseOfData")
    .AddField(DWORD, "ImageBase")
    .AddField(DWORD, "SectionAlignment")
    .AddField(DWORD, "FileAlignment")
    .AddField(WORD, "MajorOperatingSystemVersion")
    .AddField(WORD, "MinorOperatingSystemVersion")
    .AddField(WORD, "MajorImageVersion")
    .AddField(WORD, "MinorImageVersion")
    .AddField(WORD, "MajorSubsystemVersion")
    .AddField(WORD, "MinorSubsystemVersion")
    .AddField(DWORD, "Win32VersionValue")
    .AddField(DWORD, "SizeOfImage")
    .AddField(DWORD, "SizeOfHeaders")
    .AddField(DWORD, "CheckSum")
    .AddField(WORD, "Subsystem")
    .AddField(WORD, "DllCharacteristics")
    .AddField(DWORD, "SizeOfStackReserve")
    .AddField(DWORD, "SizeOfStackCommit")
    .AddField(DWORD, "SizeOfHeapReserve")
    .AddField(DWORD, "SizeOfHeapCommit")
    .AddField(DWORD, "LoaderFlags")
    .AddField(DWORD, "NumberOfRvaAndSizes")
    .AddField(std::make_shared<StaticArrayDetail>(std::make_shared<StructureDetail>(_IMAGE_DATA_DIRECTORY), 16), "DataDirectory")
    ;

  StructureDetail _IMAGE_NT_HEADERS("_IMAGE_NT_HEADERS");
  _IMAGE_NT_HEADERS
    .AddField(DWORD, "Signature")
    .AddField(std::make_shared<StructureDetail>(_IMAGE_FILE_HEADER), "FileHeader")
    .AddField(std::make_shared<StructureDetail>(_IMAGE_OPTIONAL_HEADER), "OptionalHeader")
    ;

  StructureDetail _IMAGE_DOS_HEADER("_IMAGE_DOS_HEADER");
  _IMAGE_DOS_HEADER
    .AddField(WORD, "e_magic")
    .AddField(WORD, "e_cblp")
    .AddField(WORD, "e_cp")
    .AddField(WORD, "e_crlc")
    .AddField(WORD, "e_cparhdr")
    .AddField(WORD, "e_minalloc")
    .AddField(WORD, "e_maxalloc")
    .AddField(WORD, "e_ss")
    .AddField(WORD, "e_sp")
    .AddField(WORD, "e_csum")
    .AddField(WORD, "e_ip")
    .AddField(WORD, "e_cs")
    .AddField(WORD, "e_lfarlc")
    .AddField(WORD, "e_ovno")
    .AddField(std::make_shared<StaticArrayDetail>(WORD, 4), "e_res")
    .AddField(WORD, "e_oemid")
    .AddField(WORD, "e_oeminfo")
    .AddField(std::make_shared<StaticArrayDetail>(WORD, 10), "e_res2")
    .AddField(LONG, "e_lfanew")
    ;

  BOOST_REQUIRE(_IMAGE_DOS_HEADER.IsValid());
  BOOST_REQUIRE(_IMAGE_NT_HEADERS.IsValid());

  BOOST_CHECK(_IMAGE_DOS_HEADER.GetSize() == 0x40);

  std::cout << _IMAGE_DOS_HEADER.Dump() << std::endl;
  std::cout << _IMAGE_NT_HEADERS.Dump() << std::endl;

  TypedValueDetail e_lfanew;
  BOOST_CHECK(_IMAGE_DOS_HEADER.GetFieldByOffset(0x3c, e_lfanew));
  BOOST_CHECK(e_lfanew.GetValue().GetName() == "e_lfanew");
}

BOOST_AUTO_TEST_SUITE_END()