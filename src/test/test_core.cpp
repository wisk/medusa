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

  auto WORD = std::make_shared<TypeDetail>("WORD", TypeDetail::IntegerType, 16);
  auto LONG = std::make_shared<TypeDetail>("LONG", TypeDetail::IntegerType, 32);
  StructureDetail IMAGE_DOS_HEADER("IMAGE_DOS_HEADER");
  IMAGE_DOS_HEADER
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

  BOOST_REQUIRE(IMAGE_DOS_HEADER.IsValid());

  BOOST_CHECK(IMAGE_DOS_HEADER.GetSize() == 0x40);

  std::cout << IMAGE_DOS_HEADER.Dump() << std::endl;

  TypedValueDetail e_lfanew;
  BOOST_CHECK(IMAGE_DOS_HEADER.GetFieldByOffset(0x3c, e_lfanew));
  BOOST_CHECK(e_lfanew.GetValue().GetName() == "e_lfanew");
}

BOOST_AUTO_TEST_SUITE_END()