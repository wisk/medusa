#define BOOST_TEST_MODULE TestEmulation
#include <boost/test/unit_test.hpp>

#include <medusa/module.hpp>
#include <medusa/medusa.hpp>
#include <medusa/emulation.hpp>
#include <medusa/execution.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(emulation_test_suite)

BOOST_AUTO_TEST_CASE(emul_interpreter_arm_test_case)
{
  using namespace medusa;

  BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/hello_world.elf.arm-7";
  std::cout << "emulating program: " << pSample << std::endl;

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

  auto& rDoc = Core.GetDocument();
  auto StartAddr = rDoc.GetAddressFromLabelName("start");
  auto ArchTag = rDoc.GetArchitectureTag(StartAddr);
  auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
  BOOST_REQUIRE(spArch != nullptr);
  auto Mode = rDoc.GetMode(StartAddr);
  BOOST_REQUIRE(Mode != 0);

  auto spOs = ModuleManager::Instance().GetOperatingSystem(rDoc.GetOperatingSystemName());

  std::vector<std::string> Args;
  Args.push_back(pSample);
  std::vector<std::string> Envp;

  Execution Exec(rDoc);
  BOOST_REQUIRE(Exec.Initialize(Args, Envp, SAMPLES_DIR));

  char const* pEmulatorType = "interpreter";

  std::cout << "Using emulator type: " << pEmulatorType << std::endl;
  BOOST_REQUIRE(Exec.SetEmulator(pEmulatorType));

  BOOST_REQUIRE(Exec.HookFunction("__libc_start_main", [](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const&)
  {
    std::cout << "[__libc_start_main] try to execute R0 (main)" << std::endl;
    u32 MainAddr = 0;

    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();

    u32 R0 = rCpuInfo.ConvertNameToIdentifier("r0");
    u32 PC = rCpuInfo.ConvertNameToIdentifier("pc");

    BOOST_REQUIRE(R0 != 0 && PC != 0);

    if (!pCpuCtxt->ReadRegister(R0, MainAddr))
      return;
    if (!pCpuCtxt->WriteRegister(PC, MainAddr))
      return;

  }));

  bool PutsCalled = false;
  BOOST_REQUIRE(Exec.HookFunction("puts", [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const&)
  {
    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();
    u32 R0 = rCpuInfo.ConvertNameToIdentifier("r0");
    u32 LR = rCpuInfo.ConvertNameToIdentifier("lr");
    u32 PC = rCpuInfo.ConvertNameToIdentifier("pc");

    BOOST_REQUIRE(R0 != 0 && LR != 0 && PC != 0);

    u32 ParamAddr = 0;
    if (!pCpuCtxt->ReadRegister(R0, ParamAddr))
    {
      std::cout << "[puts] failed to read parameter" << std::endl;
      return;
    }

    std::string Param;
    u64 ParamOff = 0;
    char CurChr;
    while (pMemCtxt->ReadMemory(ParamAddr + ParamOff, &CurChr, 1))
    {
      if (CurChr == '\0')
        break;
      Param += CurChr;
      ParamOff += 1;
    }

    std::cout << "[puts] param: \"" << Param << "\"" << std::endl;

    u32 RetAddr = 0;
    if (!pCpuCtxt->ReadRegister(LR, RetAddr))
      return;
    if (!pCpuCtxt->WriteRegister(PC, RetAddr))
      return;
    PutsCalled = true;
  }));

  BOOST_REQUIRE(Exec.HookFunction("abort", [](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const&)
  {
    std::cout << "[abort]" << std::endl;
  }));

  Exec.HookInstruction([&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
  {
    std::cout << pCpuCtxt->ToString() << std::endl;
    auto spInsn = std::dynamic_pointer_cast<Instruction>(Core.GetCell(rAddr));
    auto const& rModMgr = ModuleManager::Instance();
    auto spArch = rModMgr.GetArchitecture(spInsn->GetArchitectureTag());
    PrintData PD;
    PD(rAddr);
    spArch->FormatCell(rDoc, rAddr, *spInsn, PD);
    std::cout << PD.GetTexts() << std::endl;
    for (auto e : spInsn->GetSemantic())
      std::cout << e->ToString() << std::endl;
  });

  Exec.Execute(StartAddr);

  BOOST_CHECK(PutsCalled);

  Core.CloseDocument();
}

//BOOST_AUTO_TEST_CASE(emul_interpreter_x86_64_test_case)
//{
//  using namespace medusa;
//
//  BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");
//
//  Medusa Core;
//
//  auto const pSample = SAMPLES_DIR "/exe/hello_world-vs2012.pe.x86-64";
//  std::cout << "emulating program: " << pSample << std::endl;
//
//  try
//  {
//    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
//    BOOST_REQUIRE(Core.NewDocument(
//      spFileBinStrm,
//      [&](Path& rDbPath, std::list<Medusa::Filter> const&)
//    {
//      rDbPath = "hello_world-vs2012.pe.x86-64_";
//      return true;
//    }));
//  }
//  catch (Exception const& e)
//  {
//    std::cerr << e.What() << std::endl;
//    BOOST_REQUIRE(0);
//  }
//
//  Core.WaitForTasks();
//
//  auto& rDoc = Core.GetDocument();
//  auto StartAddr = rDoc.GetAddressFromLabelName("start");
//  auto spStartInsn = std::dynamic_pointer_cast<Instruction>(rDoc.GetCell(StartAddr));
//  auto ArchTag = spStartInsn->GetArchitectureTag();
//  auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
//  BOOST_REQUIRE(spArch != nullptr);
//  auto Mode = spStartInsn->GetMode();
//  BOOST_REQUIRE(Mode != 0);
//
//  auto spOs = ModuleManager::Instance().GetOperatingSystem(rDoc.GetOperatingSystemName());
//
//  std::vector<std::string> Args;
//  Args.push_back(pSample);
//  std::vector<std::string> Envp;
//
//  Execution Exec(rDoc, spArch, spOs);
//  BOOST_REQUIRE(Exec.Initialize(Mode, Args, Envp, SAMPLES_DIR));
//
//  char const* pEmulatorType = "interpreter";
//
//  std::cout << "Using emulator type: " << pEmulatorType << std::endl;
//  BOOST_REQUIRE(Exec.SetEmulator(pEmulatorType));
//
//  auto pMainCpuCtxt = Exec.GetCpuContext();
//  BOOST_REQUIRE(pMainCpuCtxt != nullptr);
//
//  u32 RAX = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rax");
//  u32 RCX = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rcx");
//  u32 RDX = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rdx");
//  u32 R8  = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("r8");
//  u32 RSP = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rsp");
//  u32 RIP = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rip");
//
//  BOOST_REQUIRE(RAX != 0 && RCX != 0 && RDX != 0 && R8 != 0 && RSP != 0 && RIP != 0);
//
//  auto Ret = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 StkAddr = 0;
//    u64 RetAddr = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RSP, StkAddr));
//    BOOST_REQUIRE(pMemCtxt->ReadMemory(StkAddr, RetAddr));
//
//    std::cout << "[ret] " << Exec.GetHookName() << " called, jumping to " << std::hex << RetAddr << std::endl;
//
//    BOOST_REQUIRE(pCpuCtxt->WriteRegister(RIP, RetAddr));
//    StkAddr += 8;
//    BOOST_REQUIRE(pCpuCtxt->WriteRegister(RSP, StkAddr));
//  };
//
//  auto RetValue = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr, u64 Value)
//  {
//    std::cout << "[retval] returning: " << Value << std::endl;
//    u64 OneVal = Value;
//    BOOST_REQUIRE(pCpuCtxt->WriteRegister(RAX, OneVal));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto Ret0 = std::bind(RetValue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 0);
//  auto Ret1 = std::bind(RetValue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 1);
//  auto Ret4 = std::bind(RetValue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 4);
//  auto Ret65001 = std::bind(RetValue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 65001);
//
//  auto RetParam0 = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 Param0 = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RCX, Param0));
//    std::cout << "[retparam] returning first parameter: " << std::hex << Param0 << std::endl;
//    BOOST_REQUIRE(pCpuCtxt->WriteRegister(RAX, Param0));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto AllocR8 = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    static u64 HeapAllocBase = 0xbeef0000;
//
//    u64 AllocSize = 0;
//    u64 AllocAddr = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(R8, AllocSize));
//
//    void* pRawMem = nullptr;
//    BOOST_REQUIRE(pMemCtxt->AllocateMemory(HeapAllocBase, static_cast<u32>(AllocSize), &pRawMem));
//    AllocAddr = HeapAllocBase;
//    HeapAllocBase += AllocSize;
//
//    std::cout << "[alloc_r8] returning allocated buffer: " << std::hex << AllocAddr << ", size: " << AllocSize << std::endl;
//    BOOST_REQUIRE(pCpuCtxt->WriteRegister(RAX, AllocAddr));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  std::map<u32, u64> TlsMap;
//
//  auto FakeTlsGetValue = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 Param0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RCX, Param0));
//    auto it = TlsMap.find(static_cast<u32>(Param0));
//    if (it == std::end(TlsMap))
//    {
//      Ret0(pCpuCtxt, pMemCtxt, rAddr);
//      return;
//    }
//    std::cout << std::hex << "[TlsGetValue] " << Param0 << " " << it->second << std::endl;
//    RetValue(pCpuCtxt, pMemCtxt, rAddr, it->second);
//  };
//  auto FakeTlsSetValue = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 Param0;
//    u64 Param1;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RCX, Param0));
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RDX, Param1));
//    std::cout << std::hex << "[TlsSetValue] " << Param0 << " " << Param1 << std::endl;
//    TlsMap[static_cast<u32>(Param0)] = Param1;
//    Ret1(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  // Zero out STARTUPINFOW structure to avoid issue with STARTUPINFOW::lpReserved2 when program tries to initialize C runtime
//  // ref: http://www.catch22.net/tuts/undocumented-createprocess
//  auto FakeGetStartupInfoW = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u8 FakeStartupInfoW[0x68];
//    ::memset(FakeStartupInfoW, 0x0, sizeof(FakeStartupInfoW));
//    u64 StartupInfoAddr = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RCX, StartupInfoAddr));
//    BOOST_REQUIRE(pMemCtxt->WriteMemory(StartupInfoAddr, FakeStartupInfoW, sizeof(FakeStartupInfoW)));
//    std::cout << "[GetStartupInfoW] zero out STARTUPINFOW structure: " << std::hex << StartupInfoAddr << std::endl;
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto FakeGetModuleFileNameA = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 FilenameAddr = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RDX, FilenameAddr));
//    auto FilenameLen = ::strlen(pSample) + 1;
//    BOOST_REQUIRE(pMemCtxt->WriteMemory(FilenameAddr, pSample, static_cast<u32>(FilenameLen)));
//    std::cout << "[GetModuleFileNameA] write \"" << pSample << "\"" << std::endl;
//
//    RetValue(pCpuCtxt, pMemCtxt, rAddr, FilenameLen - 1);
//  };
//
//  u64 EnvAddr = 0x44440000;
//  auto FakeGetEnvironmentStringsW = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    void* pRawEnv;
//    BOOST_REQUIRE(pMemCtxt->AllocateMemory(EnvAddr, 2, &pRawEnv));
//    ::memset(pRawEnv, 0x0, 2);
//
//    std::cout << "[GetEnvironmentStringsW] returns empty environ" << std::endl;
//    BOOST_REQUIRE(pCpuCtxt->WriteRegister(RAX, EnvAddr));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//  auto FakeFreeEnvironmentStringsW = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    void* pRawEnv;
//    BOOST_REQUIRE(pMemCtxt->AllocateMemory(EnvAddr, 2, &pRawEnv));
//    ::memset(pRawEnv, 0x0, 2);
//
//    std::cout << "[FreeEnvironmentStringsW] free environ" << std::endl;
//    u64 FakeEnvAddr = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RCX, FakeEnvAddr));
//    BOOST_REQUIRE(pMemCtxt->FreeMemory(FakeEnvAddr));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto FakeWideCharToMultiByte = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 StkAddr = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RSP, StkAddr));
//    u64 Param5 = 0;
//    BOOST_REQUIRE(pMemCtxt->ReadMemory(StkAddr + 0x8 + 0x20, Param5));
//    std::cout << "[WideCharToMultiByte] lpMultiByteStr = " << std::hex << Param5 << std::endl;
//
//    if (Param5 != 0)
//    {
//      std::cout << "[WideCharToMultiByte] write \\x00" << std::endl;
//      BOOST_REQUIRE(pMemCtxt->WriteMemory(Param5, "\x00", 1));
//    }
//
//    std::cout << "[WideCharToMultiByte] returns 1" << std::endl;
//
//    Ret1(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto FakeHeapSize = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 Param0 = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RCX, Param0));
//
//    void* pAddr;
//    u32 Size;
//    BOOST_REQUIRE(pMemCtxt->FindMemory(Param0, pAddr, Size));
//    u64 MemSize = Size;
//    BOOST_REQUIRE(pCpuCtxt->WriteRegister(RAX, MemSize));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto FakeMessageBoxA = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 Param1 = 0;
//    BOOST_REQUIRE(pCpuCtxt->ReadRegister(RDX, Param1));
//
//    std::string lpText;
//    u64 ParamOff = 0;
//    char CurChr;
//    while (pMemCtxt->ReadMemory(Param1 + ParamOff, &CurChr, 1))
//    {
//      if (CurChr == '\0')
//        break;
//      lpText += CurChr;
//      ParamOff += 1;
//    }
//
//    std::cout << "[MessageBoxA] lpText = \"" << lpText << "\"" << std::endl;
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetSystemTimeAsFileTime", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetCurrentThreadId", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetCurrentProcessId", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!QueryPerformanceCounter", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetStartupInfoW", FakeGetStartupInfoW));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetProcessHeap", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!HeapAlloc", AllocR8));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!HeapSize", FakeHeapSize));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!HeapFree", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!EncodePointer", RetParam0));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!DecodePointer", RetParam0));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetModuleHandleW", Ret0));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetModuleHandleExW", Ret0));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetProcAddress", Ret0));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!InitializeCriticalSectionAndSpinCount", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!EnterCriticalSection", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!LeaveCriticalSection", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!DeleteCriticalSection", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!Sleep", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!TlsAlloc", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!TlsFree", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!TlsGetValue", FakeTlsGetValue));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!TlsSetValue", FakeTlsSetValue));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetLastError", Ret0));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!SetLastError", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetStdHandle", Ret));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetFileType", Ret4));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetCommandLineA", Ret0));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetEnvironmentStringsW", FakeGetEnvironmentStringsW));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!FreeEnvironmentStringsW", FakeFreeEnvironmentStringsW));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetModuleFileNameA", FakeGetModuleFileNameA));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!WideCharToMultiByte", FakeWideCharToMultiByte));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!SetUnhandledExceptionFilter", Ret0));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!HeapSize", FakeHeapSize));
//  BOOST_REQUIRE(Exec.HookFunction("kernel32.dll!GetACP", Ret65001));
//
//  BOOST_REQUIRE(Exec.HookFunction("user32.dll!MessageBoxA", FakeMessageBoxA));
//#include <medusa/instruction.hpp>
//  Exec.HookInstruction([&](CpuContext* cpu, MemoryContext* mem, Address const& a)
//  {
//    auto insn = std::dynamic_pointer_cast<medusa::Instruction>(Core.GetCell(a));
//    std::cout << cpu->ToString() << std::endl;
//    if (insn)
//      std::cout << insn->ToString() << std::endl;
//  });
//
//  Exec.Execute(StartAddr);
//
//  Core.CloseDocument();
//}

BOOST_AUTO_TEST_SUITE_END()