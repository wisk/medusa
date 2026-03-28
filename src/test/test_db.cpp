#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <chrono>
#include <string>

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
  if (spDb == nullptr)
  {
    WARN("Database module '" << rDatabaseModuleName << "' not available");
    return;
  }

  auto TempBaseFile = std::filesystem::absolute(std::filesystem::temp_directory_path() / std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
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

