#define BOOST_TEST_MODULE TestBinding
#include <boost/test/unit_test.hpp>

#include <medusa/medusa.hpp>
#include <medusa/module.hpp>

#include <cstdlib>

BOOST_AUTO_TEST_SUITE(binding_test_suite)

BOOST_AUTO_TEST_CASE(bind_python_test_case)
{
  using namespace medusa;

  Log::SetLog([](std::string const& rMsg)
  {
    std::cout << rMsg << std::flush;
  });

  BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/hello_world.elf.arm-7";
  std::cout << "disassembling program: " << pSample << std::endl;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    BOOST_REQUIRE(Core.NewDocument(
      spFileBinStrm, true,
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

  BOOST_REQUIRE(spPython->Bind(Core));

  BOOST_REQUIRE(spPython->Execute("print 'testing python binding'"));
  BOOST_REQUIRE(spPython->Execute("doc = pydusa.core.document"));
  BOOST_REQUIRE(spPython->Execute("print doc"));
  BOOST_REQUIRE(spPython->Execute("for ma in doc.memory_areas: print ma"));
  BOOST_REQUIRE(spPython->Execute("for lbl in doc.labels: print lbl"));
  BOOST_REQUIRE(!spPython->Execute("54/0"));

  BOOST_REQUIRE(spPython->Execute("a0 = pydusa.Address(0x11223344)"));
  BOOST_REQUIRE(spPython->Execute("a1 = pydusa.Address(0x1122, 0x33445566778899aa)"));
  BOOST_REQUIRE(spPython->Execute("assert(str(a0) == '0000000011223344')"));
  BOOST_REQUIRE(spPython->Execute("assert(str(a1) == '1122:33445566778899aa')"));

  BOOST_REQUIRE(spPython->Execute("start_addr = doc.get_label_addr('start')"));
  BOOST_REQUIRE(spPython->Execute("start_lbl  = doc.get_label(start_addr)"));
  BOOST_REQUIRE(spPython->Execute("start_insn = pydusa.core.get_insn(start_addr)"));
  BOOST_REQUIRE(spPython->Execute("print start_addr, start_lbl, start_insn"));

  BOOST_REQUIRE(spPython->Execute("for expr in start_insn.sem: print expr"));
  BOOST_REQUIRE(spPython->Execute("for oprd in start_insn.oprds: print oprd"));

  BOOST_REQUIRE(spPython->Unbind(Core));
  BOOST_REQUIRE(Core.CloseDocument());
}

BOOST_AUTO_TEST_SUITE_END()
