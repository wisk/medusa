#define BOOST_TEST_MODULE TestLoader
#include <boost/test/unit_test.hpp> 

#include <medusa/loader.hpp>
#include <medusa/module.hpp>

BOOST_AUTO_TEST_SUITE(loader_test_suite)

BOOST_AUTO_TEST_CASE(ldr_boot_sector_test_case)
{
}

BOOST_AUTO_TEST_CASE(ldr_elf_test_case)
{
}

BOOST_AUTO_TEST_CASE(ldr_gameboy_test_case)
{
}

BOOST_AUTO_TEST_CASE(ldr_macho_test_case)
{
}

BOOST_AUTO_TEST_CASE(ldr_pe_test_case)
{
}

BOOST_AUTO_TEST_CASE(ldr_raw_test_case)
{
}

// FIXME(wisk): find a valid sample
//BOOST_AUTO_TEST_CASE(ldr_st62_test_case)
//{
//  auto& rModMgr = medusa::ModuleManager::Instance();
//  auto pSt62Getter = rModMgr.LoadModule<medusa::TGetLoader>(".", "st62");
//  BOOST_REQUIRE(pSt62Getter != nullptr);
//  auto pSt62Ldr = pSt62Getter();
//
//  {
//    medusa::MemoryBinaryStream MemBinStrm("\x19\x0c", 2);
//    BOOST_CHECK(!pSt62Ldr->IsCompatible(MemBinStrm));
//  }
//
//  {
//    auto const pSample = SAMPLES_DIR "/exe/fw.st62rom.st62";
//    medusa::FileBinaryStream FileBinStrm(pSample);
//    BOOST_CHECK(pSt62Ldr->IsCompatible(FileBinStrm));
//  }
//
//
//}

BOOST_AUTO_TEST_SUITE_END()
