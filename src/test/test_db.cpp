#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <boost/filesystem.hpp>

#include <medusa/database.hpp>
#include <medusa/module.hpp>

void TestDatabase(std::string const& rDatabaseModuleName)
{
  medusa::Log::SetLog([](std::string const& rMsg)
  {
    INFO("log: " << rMsg.c_str());
  });

  auto& rModMgr = medusa::ModuleManager::Instance();
  rModMgr.InitializeModules(".");
  auto spDb = rModMgr.MakeDatabase(rDatabaseModuleName);
  REQUIRE(spDb != nullptr);

  auto TempBaseFile = boost::filesystem::absolute(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path());
  {
    INFO("TempBaseFile: " << TempBaseFile.string());
    REQUIRE(spDb->Create(TempBaseFile, true));
  }

  medusa::Address BaseAddr(medusa::Address::LinearType, 0x7fffffffffULL);

  {
    INFO("Memory area");
    CHECK(spDb->AddMemoryArea(medusa::MemoryArea::CreateVirtual("virtual", medusa::MemoryArea::Access::Read,
      BaseAddr, 0x100
    )));
    medusa::MemoryArea DummyMemArea;
    CHECK(spDb->GetMemoryArea(BaseAddr, DummyMemArea));
    INFO(DummyMemArea.ToString());
    CHECK(DummyMemArea.GetBaseAddress() == BaseAddr);
    auto MappedMemArea = medusa::MemoryArea::CreateMapped(
      "mapped", medusa::MemoryArea::Access::Execute | medusa::MemoryArea::Access::Read,
      0x0, 0x10000, medusa::Address(medusa::Address::RelativeType, 0x1000), 0x10000
    );
    CHECK(spDb->AddMemoryArea(MappedMemArea));
    auto PhysicalMemArea = medusa::MemoryArea::CreatePhysical(
      "physical", medusa::MemoryArea::Access::Read, 0x1000, 0x2000
    );
    CHECK(spDb->AddMemoryArea(PhysicalMemArea));
    spDb->ForEachMemoryArea([&](medusa::MemoryArea const& rMemArea)
    {
      std::cout << rMemArea.ToString() << std::endl;
    });
  }

  medusa::ImageBaseType ImgBase;
  {
    INFO("Image base");
    CHECK(spDb->GetImageBase(ImgBase));
    CHECK(spDb->SetImageBase(0x400000));
    CHECK(spDb->GetImageBase(ImgBase));
    CHECK(ImgBase == 0x400000);
  }

  {
    INFO("Label");
    medusa::Address LblAddr, DummyAddr;
    medusa::Label Lbl(BaseAddr, medusa::Label::Data);
    medusa::Label DummyLbl;
    CHECK(!spDb->GetLabelAddress(DummyLbl, DummyAddr));
    CHECK(spDb->AddLabel(BaseAddr, Lbl));
    CHECK(spDb->GetLabel(BaseAddr, DummyLbl));
    CHECK(spDb->GetLabelAddress(DummyLbl, LblAddr));
    CHECK(Lbl == DummyLbl);
    CHECK(BaseAddr == LblAddr);
    spDb->ForEachLabel([&](medusa::Address const& rAddr, medusa::Label const& rLbl)
    {
      INFO("label: " << rAddr.ToString() << ": " << rLbl.GetName());
    });
  }

  {
    INFO("Comment");
    std::string Cmt0, Cmt1;
    CHECK(!spDb->GetComment(BaseAddr, Cmt0));
    CHECK(spDb->SetComment(BaseAddr, "test \\o/"));
    CHECK(spDb->GetComment(BaseAddr, Cmt0));
    CHECK(spDb->SetComment(BaseAddr, "test /o\\"));
    CHECK(spDb->GetComment(BaseAddr, Cmt1));
    CHECK(Cmt0 != Cmt1);
    CHECK(Cmt0 == "test \\o/");
    CHECK(Cmt1 == "test /o\\");
  }

  {
    INFO("Cell data");
    medusa::CellData CellData(medusa::Cell::InstructionType, 0x0, 0x5);
    medusa::CellData DummyCellData;
    medusa::Address::Vector V;
    CHECK(spDb->GetCellData(BaseAddr + 10, DummyCellData)); // return a default value
    CHECK(spDb->SetCellData(BaseAddr + 10, CellData, V, true));
    CHECK(spDb->GetCellData(BaseAddr + 10, DummyCellData));
    CHECK(spDb->SetCellData(BaseAddr + 15, CellData, V, true));
    CHECK(spDb->GetCellData(BaseAddr + 15, DummyCellData));
    CHECK(spDb->SetCellData(BaseAddr + 20, CellData, V, true));
    CHECK(spDb->GetCellData(BaseAddr + 20, DummyCellData));
  }

  {
    INFO("Addresses");

    medusa::Address FirstAddr, LastAddr;
    CHECK(spDb->GetFirstAddress(FirstAddr));
    CHECK(spDb->GetLastAddress(LastAddr));
    INFO("First address: " << FirstAddr.ToString());
    INFO("Last address: " << LastAddr.ToString());

    medusa::Address PrevAddr, NextAddr;
    CHECK(spDb->MoveAddress(BaseAddr + 15, PrevAddr, -1));
    CHECK(spDb->MoveAddress(BaseAddr + 15, NextAddr, 1));
    CHECK(PrevAddr == (BaseAddr + 10));
    CHECK(NextAddr == (BaseAddr + 20));
    INFO("Next address: " << PrevAddr.ToString());
    INFO("Previous address: " << NextAddr.ToString());

    medusa::Address PhysAddr;
    CHECK(spDb->TranslateAddress(medusa::Address(medusa::Address::RelativeType, 0x1005), medusa::Address::PhysicalType, PhysAddr));
    CHECK(PhysAddr.GetOffset() == 0x5);
    medusa::Address LinAddr;
    CHECK(spDb->TranslateAddress(PhysAddr, medusa::Address::LinearType, LinAddr));
    CHECK(LinAddr.GetOffset() == (ImgBase + 0x1000 + 0x5));
  }
}

TEST_CASE("memory", "[db_memory]")
{
  INFO("Testing Memory database");
  TestDatabase("Memory");
}

TEST_CASE("sqlite3", "[db_sqlite3]")
{
  INFO("Testing SQLite3 database");
  TestDatabase("Sqlite3");
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
