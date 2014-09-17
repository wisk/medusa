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
  StructureDetail SD("IMAGE_DOS_HEADER");
  SD
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_magic")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_cblp")))
    .AddField(new TypedValueDetail(WORD, ValueDetail("e_cp")))
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
    ;
  std::cout << SD.Dump() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()