#define BOOST_TEST_MODULE TestLoader
#include <boost/test/unit_test.hpp> 

#include <medusa/loader.hpp>
#include <medusa/module.hpp>
#include <boost/filesystem.hpp>

BOOST_AUTO_TEST_SUITE(loader_test_suite)

BOOST_AUTO_TEST_CASE(ldr_boot_sector_test_case) {
}

BOOST_AUTO_TEST_CASE(ldr_elf_test_case) {
}

BOOST_AUTO_TEST_CASE(ldr_gameboy_test_case) {
}

BOOST_AUTO_TEST_CASE(ldr_macho_test_case) {
}

BOOST_AUTO_TEST_CASE(ldr_pe_test_case) {
}

BOOST_AUTO_TEST_CASE(ldr_raw_test_case) {
}

BOOST_AUTO_TEST_CASE(ldr_dos_test_case) {
    BOOST_MESSAGE("Testing dos loader");

    auto const SampleDosPascalMario = SAMPLES_DIR "/exe/mario.dos.x16.exe";
    using namespace medusa;
    auto& rModMgr = medusa::ModuleManager::Instance();
    rModMgr.UnloadModules();
    {
        auto pLdrDos = rModMgr.LoadModule<medusa::TGetLoader>(".", "dos");
        
        BOOST_CHECK(pLdrDos != nullptr);
        rModMgr.UnloadModules();
    }

    {
        FileBinaryStream *spFileBinStrm = new FileBinaryStream(SampleDosPascalMario);
        rModMgr.LoadModules(".", *spFileBinStrm);
        auto const& all_ldrs = rModMgr.GetLoaders();
        bool IsDosModuleExists = false;
        for (auto &current_ldr : all_ldrs) {
            BOOST_MESSAGE("current loader: " + current_ldr->GetName());
            if (current_ldr->GetName().find("MS-DOS")!=-1) {
                IsDosModuleExists = true;
            }
        }
        BOOST_CHECK(IsDosModuleExists);
        spFileBinStrm->Close();
        delete spFileBinStrm;
    }
    
    

}

BOOST_AUTO_TEST_SUITE_END()