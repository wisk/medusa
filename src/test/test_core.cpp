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

  TypeDetail WORD("WORD", TypeDetail::IntegerType, 16);
  TypeDetail LONG("LONG", TypeDetail::IntegerType, 32);
  StructureDetail IMAGE_DOS_HEADER("IMAGE_DOS_HEADER");
  IMAGE_DOS_HEADER
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_magic")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_cblp")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_cp")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_crlc")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_cparhdr")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_minalloc")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_maxalloc")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_ss")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_sp")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_csum")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_ip")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_cs")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_lfarlc")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_ovno")))
    .AddField(new StaticArrayDetail(TypedValueDetail(WORD, ValueDetail("e_res")), 4))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_oemid")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_oeminfo")))
    .AddField(new StaticArrayDetail(TypedValueDetail(WORD, ValueDetail("e_res2")), 10))
    .AddField(new TypedValueDetail(LONG, ValueDetail("e_lfanew")))
    ;

  BOOST_REQUIRE(IMAGE_DOS_HEADER.IsValid());

  std::cout << IMAGE_DOS_HEADER.Dump() << std::endl;

  auto p__e_lfanew = IMAGE_DOS_HEADER.GetFieldByOffset(0x3c);
  BOOST_CHECK(p__e_lfanew != nullptr);
  BOOST_CHECK(p__e_lfanew->GetName() == "e_lfanew");
}

BOOST_AUTO_TEST_SUITE_END()