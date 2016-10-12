#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <boost/filesystem.hpp>

#include <medusa/database.hpp>
#include <medusa/module.hpp>

TEST_CASE("load", "[db_soci]") {
    INFO("Testing SOCI database");

    auto& rModMgr = medusa::ModuleManager::Instance();
    rModMgr.LoadDatabases(".");
    auto spSociDb = rModMgr.GetDatabase("SOCI");
    REQUIRE(spSociDb != nullptr);

    auto TempBaseFile = boost::filesystem::absolute(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path());

    REQUIRE(spSociDb->Create(TempBaseFile, true));

    medusa::Address BaseAddr(medusa::Address::VirtualType, 0x7fffffffffULL);

    INFO("Memory area");
    CHECK(spSociDb->AddMemoryArea(medusa::MemoryArea::CreateVirtual("virtual", medusa::MemoryArea::Read,
      BaseAddr, 0x100
    )));
    medusa::MemoryArea DummyMemArea;
    CHECK(spSociDb->GetMemoryArea(BaseAddr, DummyMemArea));
    INFO(DummyMemArea.Dump());
    CHECK(DummyMemArea.GetBaseAddress() == BaseAddr);
    auto MappedMemArea = medusa::MemoryArea::CreateMapped(
      "mapped", medusa::MemoryArea::Execute | medusa::MemoryArea::Read,
      0x0, 0x10000, medusa::Address(medusa::Address::RelativeType, 0x1000), 0x10000
    );
    CHECK(spSociDb->AddMemoryArea(MappedMemArea));
    auto PhysicalMemArea = medusa::MemoryArea::CreatePhysical(
      "physical", medusa::MemoryArea::Read, 0x1000, 0x2000
    );
    CHECK(spSociDb->AddMemoryArea(PhysicalMemArea));
    spSociDb->ForEachMemoryArea([&](medusa::MemoryArea const& rMemArea)
    {
      INFO(rMemArea.ToString());
    });

    INFO("Image base");
    medusa::ImageBaseType ImgBase;
    CHECK(!spSociDb->GetImageBase(ImgBase));
    CHECK(spSociDb->SetImageBase(0x400000));
    CHECK(spSociDb->GetImageBase(ImgBase));
    CHECK(ImgBase == 0x400000);

    INFO("Label");
    medusa::Address LblAddr;
    medusa::Label Lbl(BaseAddr, medusa::Label::Data);
    medusa::Label DummyLbl;
    CHECK(!spSociDb->GetLabelAddress(DummyLbl, BaseAddr));
    CHECK(spSociDb->AddLabel(BaseAddr, Lbl));
    CHECK(spSociDb->GetLabel(BaseAddr, DummyLbl));
    CHECK(spSociDb->GetLabelAddress(DummyLbl, LblAddr));
    CHECK(Lbl == DummyLbl);
    CHECK(BaseAddr == LblAddr);

    INFO("Comment");
    std::string Cmt0, Cmt1;
    CHECK(!spSociDb->GetComment(BaseAddr, Cmt0));
    CHECK(spSociDb->SetComment(BaseAddr, "test \\o/"));
    CHECK(spSociDb->GetComment(BaseAddr, Cmt0));
    CHECK(spSociDb->SetComment(BaseAddr, "test /o\\"));
    CHECK(spSociDb->GetComment(BaseAddr, Cmt1));
    CHECK(Cmt0 != Cmt1);
    CHECK(Cmt0 == "test \\o/");
    CHECK(Cmt1 == "test /o\\");

    INFO("Cell data");
    medusa::CellData CellData(medusa::Cell::InstructionType, 0x0, 0x5);
    medusa::CellData DummyCellData;
    std::list<medusa::Address> V;
    CHECK(spSociDb->GetCellData(BaseAddr + 10, DummyCellData));
    CHECK(spSociDb->SetCellData(BaseAddr + 10, CellData, V, true));
    CHECK(spSociDb->GetCellData(BaseAddr + 10, DummyCellData));
    CHECK(spSociDb->SetCellData(BaseAddr + 15, CellData, V, true));
    CHECK(spSociDb->GetCellData(BaseAddr + 15, DummyCellData));
    CHECK(spSociDb->SetCellData(BaseAddr + 20, CellData, V, true));
    CHECK(spSociDb->GetCellData(BaseAddr + 20, DummyCellData));

    medusa::Address FirstAddr, LastAddr;
    CHECK(spSociDb->GetFirstAddress(FirstAddr));
    CHECK(spSociDb->GetLastAddress(LastAddr));
    INFO("First address: " << FirstAddr.ToString());
    INFO("Last address: " << LastAddr.ToString());
}

//TEST_CASE("all database modules", "[db_*]") {
//    using namespace medusa;
//    auto& rModMgr = medusa::ModuleManager::Instance();
//    rModMgr.UnloadModules();
//    rModMgr.LoadDatabases(".");
//    auto const& all_dbs = rModMgr.GetDatabases();
//    
//    auto const pSample = SAMPLES_DIR "/exe/hello_world.elf.arm-7";
//    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
//    INFO("Using samples path \"" SAMPLES_DIR "\"");
//    for (auto &db : all_dbs)
//    {
//        INFO("Current database name: " + db->GetName());
//
//        auto TempBaseFile = boost::filesystem::absolute(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path());
//        INFO("Created unique  path: " + TempBaseFile.string());
//        CHECK(db->Create(TempBaseFile,true));
//        
//        db->SetBinaryStream(spFileBinStrm);
//        medusa::Label SampleLabel("test string",Label::Type::String);
//        medusa::Address SampleAddress;
//        db->AddLabel(SampleAddress,SampleLabel);
//        CHECK(db->Flush());
//        CHECK(db->Close());
//        
//        CHECK(db->Open(TempBaseFile));
//        medusa::Label OutLabel;
//        CHECK(db->GetLabel(SampleAddress,OutLabel));
//        CHECK(OutLabel.GetName() == SampleLabel.GetName());
//        CHECK(db->Close());
//
//        boost::filesystem::remove(TempBaseFile);
//    }
//    spFileBinStrm->Close();
//}
