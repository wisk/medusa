#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <medusa/loader.hpp>
#include <medusa/module.hpp>

TEST_CASE("", "[ldr_boot_sector]")
{
}

TEST_CASE("", "[ldr_elf]")
{
}

TEST_CASE("", "[ldr_gameboy]")
{
}

TEST_CASE("", "[ldr_macho]")
{
}

TEST_CASE("", "[ldr_pe]")
{
}

TEST_CASE("", "[ldr_raw]")
{
}

// FIXME(wisk): find a valid sample
//TEST_CASE("", "[ldr_st62]")
//{
//  auto& rModMgr = medusa::ModuleManager::Instance();
//  auto pSt62Getter = rModMgr.LoadModule<medusa::TGetLoader>(".", "st62");
//  REQUIRE(pSt62Getter != nullptr);
//  auto pSt62Ldr = pSt62Getter();
//
//  {
//    medusa::MemoryBinaryStream MemBinStrm("\x19\x0c", 2);
//    CHECK(!pSt62Ldr->IsCompatible(MemBinStrm));
//  }
//
//  {
//    auto const pSample = SAMPLES_DIR "/exe/fw.st62rom.st62";
//    medusa::FileBinaryStream FileBinStrm(pSample);
//    CHECK(pSt62Ldr->IsCompatible(FileBinStrm));
//  }
//
//
//}
