#define BOOST_TEST_MODULE TestDatabase
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <medusa/database.hpp>
#include <medusa/module.hpp>

BOOST_AUTO_TEST_SUITE(database_test_suite)

BOOST_AUTO_TEST_CASE(db_text_test_case) {
    BOOST_MESSAGE("Testing text database");

    auto& rModMgr = medusa::ModuleManager::Instance();
    auto pDbText = rModMgr.LoadModule<medusa::TGetDatabase>(".", "text");
    BOOST_CHECK(pDbText != nullptr);
}

BOOST_AUTO_TEST_CASE(db_test_all_database_modules) {
    using namespace medusa;
    auto& rModMgr = medusa::ModuleManager::Instance();
    rModMgr.UnloadModules();
    rModMgr.LoadDatabases(".");
    auto const& all_dbs = rModMgr.GetDatabases();
    
    auto const pSample = SAMPLES_DIR "/exe/hello_world.elf.arm-7";
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");
    for (auto &db : all_dbs) {
        BOOST_MESSAGE("Current database name: " + db->GetName());
        boost::filesystem::path tempBaseFile =  boost::filesystem::absolute(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path());
        BOOST_MESSAGE("Created unique  path: " + tempBaseFile.string());
        BOOST_CHECK(db->Create(tempBaseFile,true));
        db->SetBinaryStream(spFileBinStrm);
        medusa::Label  SampleLabel("test string",Label::Type::String);
        medusa::Address SampleAddress;
        db->AddLabel(SampleAddress,SampleLabel);
        BOOST_CHECK(db->Flush());
        BOOST_CHECK(db->Close());
        BOOST_CHECK(db->Open(tempBaseFile));
        medusa::Label OutLabel;
        BOOST_CHECK(db->GetLabel(SampleAddress,OutLabel));
        BOOST_CHECK(OutLabel.GetName()==SampleLabel.GetName());
        BOOST_CHECK(db->Close());
        boost::filesystem::remove(tempBaseFile);
    }
    spFileBinStrm->Close();
}



BOOST_AUTO_TEST_SUITE_END()
