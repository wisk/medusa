#define BOOST_TEST_MODULE TestCore
#include <boost/test/unit_test.hpp>

#include <medusa/medusa.hpp>
#include <medusa/detail.hpp>
#include <medusa/disassembly_view.hpp>

#include <iostream>

class TextFullDisassemblyView : public medusa::FullDisassemblyView
{
public:
  TextFullDisassemblyView(medusa::Medusa& rCore, medusa::u32 FormatFlags, medusa::u32 Width, medusa::u32 Height, medusa::Address const& rAddress)
    : FullDisassemblyView(rCore, FormatFlags, Width, Height, rAddress)
  {}

  void Print(void)
  {
    std::cout << m_PrintData.GetTexts() << std::flush;
  }
};

BOOST_AUTO_TEST_SUITE(core_test_suite)

BOOST_AUTO_TEST_CASE(core_ap_int_test_case)
{
  using namespace medusa;

  IntType Int0(16, -1);
  BOOST_CHECK(Int0.GetUnsignedValue() == 0xffff);

  IntType Int1(16, 0xffff);
  BOOST_CHECK(Int1.GetSignedValue() == -1);

  ++Int1;
  BOOST_CHECK(Int1.GetSignedValue() == 0);

  --Int1;
  BOOST_CHECK(Int1.GetUnsignedValue() == 0xffff);
  BOOST_CHECK(Int1.GetSignedValue() == -1);
}

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
    .AddField(LONG, "e_lfanew", ValueDetail::RelativeType, _IMAGE_NT_HEADERS.GetId())
    ;

  BOOST_REQUIRE(_IMAGE_DOS_HEADER.IsValid());
  BOOST_REQUIRE(_IMAGE_NT_HEADERS.IsValid());

  BOOST_REQUIRE(_IMAGE_DOS_HEADER.GetSize() == 0x40);

  std::cout << _IMAGE_DOS_HEADER.Dump() << std::endl;
  std::cout << _IMAGE_NT_HEADERS.Dump() << std::endl;

  TypedValueDetail e_lfanew;
  BOOST_REQUIRE(_IMAGE_DOS_HEADER.GetFieldByOffset(0x3c, e_lfanew));
  BOOST_REQUIRE(e_lfanew.GetValue().GetName() == "e_lfanew");

  static u8 WinExeHdr[] = {
    0x4d, 0x5a, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8, 0x00, 0x00, 0x00,
    0x0e, 0x1f, 0xba, 0x0e, 0x00, 0xb4, 0x09, 0xcd, 0x21, 0xb8, 0x01, 0x4c, 0xcd, 0x21, 0x54, 0x68,
    0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 0x61, 0x6d, 0x20, 0x63, 0x61, 0x6e, 0x6e, 0x6f,
    0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6e, 0x20, 0x69, 0x6e, 0x20, 0x44, 0x4f, 0x53, 0x20,
    0x6d, 0x6f, 0x64, 0x65, 0x2e, 0x0d, 0x0d, 0x0a, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x5f, 0xce, 0x29, 0x64, 0x3e, 0xa0, 0x7a, 0x64, 0x3e, 0xa0, 0x7a, 0x64, 0x3e, 0xa0, 0x7a,
    0xea, 0x21, 0xb3, 0x7a, 0x71, 0x3e, 0xa0, 0x7a, 0x98, 0x1e, 0xb2, 0x7a, 0x67, 0x3e, 0xa0, 0x7a,
    0x52, 0x69, 0x63, 0x68, 0x64, 0x3e, 0xa0, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x45, 0x00, 0x00, 0x4c, 0x01, 0x05, 0x00,
    0xe3, 0x46, 0x76, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x0e, 0x01,
    0x0b, 0x01, 0x05, 0x0c, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
    0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
    0x00, 0x30, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe0, 0x50, 0x00, 0x00, 0xa4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  medusa::Medusa Core;
  BOOST_REQUIRE(Core.NewDocument(std::make_shared<medusa::MemoryBinaryStream>(WinExeHdr, sizeof(WinExeHdr))));
  auto pDosHdrStruct = new MultiCell(_IMAGE_DOS_HEADER.GetId(), MultiCell::StructType, _IMAGE_DOS_HEADER.GetSize());
  auto& rDoc = Core.GetDocument();
  BOOST_REQUIRE(rDoc.SetStructureDetail(_IMAGE_DOS_HEADER.GetId(), _IMAGE_DOS_HEADER));
  BOOST_REQUIRE(rDoc.SetStructureDetail(_IMAGE_FILE_HEADER.GetId(), _IMAGE_FILE_HEADER));
  BOOST_REQUIRE(rDoc.SetStructureDetail(_IMAGE_DATA_DIRECTORY.GetId(), _IMAGE_DATA_DIRECTORY));
  BOOST_REQUIRE(rDoc.SetStructureDetail(_IMAGE_OPTIONAL_HEADER.GetId(), _IMAGE_OPTIONAL_HEADER));
  BOOST_REQUIRE(rDoc.SetStructureDetail(_IMAGE_NT_HEADERS.GetId(), _IMAGE_NT_HEADERS));
  BOOST_REQUIRE(rDoc.SetMultiCell(rDoc.GetFirstAddress(), pDosHdrStruct, true));

  int const Step = 20;
  TextFullDisassemblyView tfdv(Core, medusa::FormatDisassembly::ShowAddress | medusa::FormatDisassembly::AddSpaceBeforeXref, 80, Step, rDoc.GetFirstAddress());
  tfdv.Refresh();
  do tfdv.Print();
  while (tfdv.MoveView(0, Step));
}

BOOST_AUTO_TEST_CASE(core_anlz_call_reg_test_case)
{
  using namespace medusa;

  BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/call_reg.pe.x86-32";
  std::cout << "analyzing program: " << pSample << std::endl;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    BOOST_REQUIRE(Core.NewDocument(
      spFileBinStrm,
      [&](Path& rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = "call_reg.pe.x86-32_";
      return true;
    }));
  }
  catch (Exception const& e)
  {
    std::cerr << e.What() << std::endl;
    BOOST_REQUIRE(0);
  }

  Core.WaitForTasks();
}

BOOST_AUTO_TEST_CASE(core_anlz_jmp_tbl_test_case)
{
  using namespace medusa;

  BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/jmp_tbl.pe.x86-32";
  std::cout << "analyzing program: " << pSample << std::endl;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    BOOST_REQUIRE(Core.NewDocument(
      spFileBinStrm,
      [&](Path& rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = "jmp_tbl.pe.x86-32_";
      return true;
    }));
  }
  catch (Exception const& e)
  {
    std::cerr << e.What() << std::endl;
    BOOST_REQUIRE(0);
  }

  Core.WaitForTasks();
}

BOOST_AUTO_TEST_SUITE_END()