#define BOOST_TEST_MODULE TestEmulation
#include <boost/test/unit_test.hpp>

#include <medusa/module.hpp>
#include <medusa/medusa.hpp>
#include <medusa/emulation.hpp>
#include <medusa/execution.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(emulation_test_suite)

BOOST_AUTO_TEST_CASE(emul_interpreter_test_case)
{
  using namespace medusa;

  Medusa Core;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>("../../../../samples/exe/ls.elf.arm-7");
    BOOST_REQUIRE(Core.NewDocument(
      spFileBinStrm,
      [&](Path& rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = "tmp";
      return true;
    }));
  }
  catch (Exception const& e)
  {
    std::cerr << e.What() << std::endl;
    BOOST_REQUIRE(0);
  }

  Core.WaitForTasks();

  auto& rDoc = Core.GetDocument();
  auto StartAddr = rDoc.GetAddressFromLabelName("start");
  auto spStartInsn = std::dynamic_pointer_cast<Instruction>(rDoc.GetCell(StartAddr));
  auto ArchTag = spStartInsn->GetArchitectureTag();
  auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
  BOOST_REQUIRE(spArch != nullptr);
  auto Mode = spStartInsn->GetMode();
  BOOST_REQUIRE(Mode != 0);

  auto spOs = ModuleManager::Instance().GetOperatingSystem(rDoc.GetOperatingSystemName());

  Execution Exec(rDoc, spArch, spOs);
  Exec.Initialize(Mode, 0x70000000, 0x10000);

  BOOST_REQUIRE(Exec.SetEmulator("interpretor"));

  Exec.Execute(StartAddr);
}

BOOST_AUTO_TEST_SUITE_END()