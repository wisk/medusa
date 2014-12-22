#define BOOST_TEST_MODULE TestEmulation
#include <boost/test/unit_test.hpp>

#include <medusa/module.hpp>
#include <medusa/medusa.hpp>
#include <medusa/emulation.hpp>
#include <medusa/execution.hpp>

#include <boost/multiprecision/cpp_int.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(emulation_test_suite)

BOOST_AUTO_TEST_CASE(emul_interpreter_test_case)
{
  using namespace medusa;

  BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");

  namespace bmp = boost::multiprecision;
  bmp::cpp_int i;
  i = 0x80000000;

  std::cout << std::hex << i << " " << bmp::msb(i) << " " << std::endl;

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/hello_world.elf.arm-7";

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
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

  std::vector<std::string> Args;
  Args.push_back(pSample);
  std::vector<std::string> Envp;

  Execution Exec(rDoc, spArch, spOs);
  Exec.Initialize(Mode, Args, Envp, SAMPLES_DIR);

  BOOST_REQUIRE(Exec.SetEmulator("interpreter"));

  Exec.HookFunction("__libc_start_main", [](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
  {
    std::cout << "[__libc_start_main] try to execute R0 (main)" << std::endl;
    u64 MainAddr = 0;

    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();

    u32 R0 = rCpuInfo.ConvertNameToIdentifier("r0");
    u32 PC = rCpuInfo.ConvertNameToIdentifier("pc");

    if (R0 == 0 || PC == 0)
      return;

    if (!pCpuCtxt->ReadRegister(R0, &MainAddr, 4))
      return;
    if (!pCpuCtxt->WriteRegister(PC, &MainAddr, 4))
      return;

  });

  Exec.Execute(StartAddr);

  Core.CloseDocument();
}

BOOST_AUTO_TEST_SUITE_END()