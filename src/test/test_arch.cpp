#define BOOST_TEST_MODULE TestArchitecture
#include <boost/test/unit_test.hpp> 

#include <medusa/module.hpp>
#include <medusa/architecture.hpp>

BOOST_AUTO_TEST_SUITE(architecture_test_suite)

BOOST_AUTO_TEST_CASE(arch_arm_test_case)
{
  BOOST_MESSAGE("Testing ARM architecture");

  auto& rModMgr = medusa::ModuleManager::Instance();
  auto pLdrArm = rModMgr.LoadModule<medusa::TGetLoader>(".", "arm");
  BOOST_CHECK(pLdrArm != nullptr);
}

BOOST_AUTO_TEST_CASE(arch_avr8_test_case)
{
}

BOOST_AUTO_TEST_CASE(arch_gameboy_test_case)
{
}

BOOST_AUTO_TEST_CASE(arch_x86_test_case)
{
}

BOOST_AUTO_TEST_SUITE_END()