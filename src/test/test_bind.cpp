#define BOOST_TEST_MODULE TestBinding
#include <boost/test/unit_test.hpp> 

#include <medusa/medusa.hpp>
#include <medusa/module.hpp>

BOOST_AUTO_TEST_SUITE(binding_test_suite)

BOOST_AUTO_TEST_CASE(bind_python_test_case)
{
  using namespace medusa;

  BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/hello_world.elf.arm-7";
  std::cout << "disassembling program: " << pSample << std::endl;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    BOOST_REQUIRE(Core.NewDocument(
      spFileBinStrm,
      [&](Path& rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = "hello_world.elf.arm-7_";
      return true;
    }));
  }
  catch (Exception const& e)
  {
    std::cerr << e.What() << std::endl;
    BOOST_REQUIRE(0);
  }

  Core.WaitForTasks();

  auto& rModMgr = ModuleManager::Instance();
  auto pGetPythonBinding = rModMgr.GetBinding("python");
  BOOST_REQUIRE(pGetPythonBinding != nullptr);

  auto spPython = std::shared_ptr<Binding>(pGetPythonBinding());
  std::cout << "binding name " << spPython->GetName() << std::endl;

  Core.CloseDocument();
}

BOOST_AUTO_TEST_SUITE_END()