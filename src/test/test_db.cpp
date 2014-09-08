#define BOOST_TEST_MODULE TestDatabase
#include <boost/test/unit_test.hpp>

#include <medusa/database.hpp>
#include <medusa/module.hpp>

BOOST_AUTO_TEST_SUITE(database_test_suite)

BOOST_AUTO_TEST_CASE(db_text_test_case)
{
  BOOST_MESSAGE("Testing text database");

  auto& rModMgr = medusa::ModuleManager::Instance();
  auto pDbText = rModMgr.LoadModule<medusa::TGetDatabase>(".", "text");
  BOOST_CHECK(pDbText != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
