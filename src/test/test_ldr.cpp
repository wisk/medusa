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

BOOST_AUTO_TEST_CASE(ldr_dos_test_case)
{
    auto const SampleDosPascalMario = SAMPLES_DIR "/exe/mario.dos.x16.exe";
    using namespace medusa;
    auto& rModMgr = medusa::ModuleManager::Instance();
    rModMgr.UnloadModules();
     auto pLdrDos = rModMgr.LoadModule<medusa::TGetLoader>(".", "dos");
     BOOST_CHECK(pLdrDos != nullptr);
    
}

BOOST_AUTO_TEST_SUITE_END()