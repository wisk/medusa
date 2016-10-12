#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <medusa/medusa.hpp>
#include <medusa/module.hpp>

#include <cstdlib>

TEST_CASE("", "[bind_python]")
{
  using namespace medusa;

  Log::SetLog([](std::string const& rMsg)
  {
    std::cout << rMsg << std::flush;
  });

  INFO("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/hello_world.elf.arm-7";
  std::cout << "disassembling program: " << pSample << std::endl;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    REQUIRE(Core.NewDocument(
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
    REQUIRE(0);
  }

  Core.WaitForTasks();

  auto& rModMgr = ModuleManager::Instance();
  auto pGetPythonBinding = rModMgr.GetBinding("python");
  REQUIRE(pGetPythonBinding != nullptr);

  auto spPython = std::shared_ptr<Binding>(pGetPythonBinding());
  std::cout << "binding name " << spPython->GetName() << std::endl;

  REQUIRE(spPython->Bind(Core));

  REQUIRE(spPython->Execute("print 'testing python binding'"));
  REQUIRE(spPython->Execute("doc = pydusa.core.document"));
  REQUIRE(spPython->Execute("print doc"));
  REQUIRE(spPython->Execute("for ma in doc.memory_areas: print ma"));
  REQUIRE(spPython->Execute("for lbl in doc.labels: print lbl"));
  REQUIRE(!spPython->Execute("54/0"));

  REQUIRE(spPython->Execute("a0 = pydusa.Address(0x11223344)"));
  REQUIRE(spPython->Execute("a1 = pydusa.Address(0x1122, 0x33445566778899aa)"));
  REQUIRE(spPython->Execute("assert(str(a0) == '0000000011223344')"));
  REQUIRE(spPython->Execute("assert(str(a1) == '1122:33445566778899aa')"));

  REQUIRE(spPython->Execute("start_addr = doc.get_label_addr('start')"));
  REQUIRE(spPython->Execute("start_lbl  = doc.get_label(start_addr)"));
  REQUIRE(spPython->Execute("start_insn = pydusa.core.get_insn(start_addr)"));
  REQUIRE(spPython->Execute("print start_addr, start_lbl, start_insn"));

  REQUIRE(spPython->Execute("for expr in start_insn.sem: print expr"));
  REQUIRE(spPython->Execute("for oprd in start_insn.oprds: print oprd"));

  REQUIRE(spPython->Unbind(Core));
  REQUIRE(Core.CloseDocument());
}
