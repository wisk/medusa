#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <medusa/module.hpp>
#include <medusa/medusa.hpp>
#include <medusa/emulation.hpp>
#include <medusa/execution.hpp>

#include <iostream>

//TEST_CASE("arm", "[emul_interpreter_arm]")
//{
//  using namespace medusa;
//
//  INFO("Using samples path \"" SAMPLES_DIR "\"");
//
//  Medusa Core;
//
//  auto const pSample = SAMPLES_DIR "/exe/hello_world.elf.arm-7";
//  std::cout << "emulating program: " << pSample << std::endl;
//
//  try
//  {
//    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
//    REQUIRE(Core.NewDocument(
//      spFileBinStrm, false,
//      [&](Path& rDbPath, std::list<Medusa::Filter> const&)
//    {
//      rDbPath = "hello_world.elf.arm-7_";
//      return true;
//    }));
//  }
//  catch (Exception const& e)
//  {
//    std::cerr << e.What() << std::endl;
//    REQUIRE(0);
//  }
//
//  Core.WaitForTasks();
//
//  auto& rDoc = Core.GetDocument();
//  auto StartAddr = rDoc.GetAddressFromLabelName("start");
//  auto ArchTag = rDoc.GetArchitectureTag(StartAddr);
//  auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
//  REQUIRE(spArch != nullptr);
//  auto Mode = rDoc.GetMode(StartAddr);
//  REQUIRE(Mode != 0);
//
//  auto spOs = ModuleManager::Instance().GetOperatingSystem(rDoc.GetOperatingSystemName());
//
//  std::vector<std::string> Args;
//  Args.push_back(pSample);
//  std::vector<std::string> Envp;
//
//  Execution Exec(rDoc);
//  REQUIRE(Exec.Initialize(Args, Envp, SAMPLES_DIR));
//
//  char const* pEmulatorType = "interpreter";
//
//  std::cout << "Using emulator type: " << pEmulatorType << std::endl;
//  REQUIRE(Exec.SetEmulator(pEmulatorType));
//
//  bool Res = 
//  REQUIRE(Exec.HookFunction("__libc_start_main", [](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const&) -> Emulator::ReturnType
//  {
//    std::cout << "[__libc_start_main] try to execute R0 (main)" << std::endl;
//    u32 MainAddr = 0;
//
//    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();
//
//    u32 R0 = rCpuInfo.ConvertNameToIdentifier("r0");
//    u32 PC = rCpuInfo.ConvertNameToIdentifier("pc");
//
//    REQUIRE(R0 != 0 && PC != 0);
//
//    if (!pCpuCtxt->ReadRegister(R0, MainAddr))
//      return Emulator::Error;
//    if (!pCpuCtxt->WriteRegister(PC, MainAddr))
//      return Emulator::Error;
//
//    return Emulator::Continue;
//  }));
//
//  bool PutsCalled = false;
//  bool IsHelloWorld = false;
//  REQUIRE(Exec.HookFunction("puts", [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const&) -> Emulator::ReturnType
//  {
//    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();
//    u32 R0 = rCpuInfo.ConvertNameToIdentifier("r0");
//    u32 LR = rCpuInfo.ConvertNameToIdentifier("lr");
//    u32 PC = rCpuInfo.ConvertNameToIdentifier("pc");
//
//    REQUIRE(R0 != 0 && LR != 0 && PC != 0);
//
//    u32 ParamAddr = 0;
//    if (!pCpuCtxt->ReadRegister(R0, ParamAddr))
//    {
//      std::cout << "[puts] failed to read parameter" << std::endl;
//      return Emulator::Error;
//    }
//
//    std::string Param;
//    u64 ParamOff = 0;
//    char CurChr;
//    while (pMemCtxt->ReadMemory(ParamAddr + ParamOff, &CurChr, 1))
//    {
//      if (CurChr == '\0')
//        break;
//      Param += CurChr;
//      ParamOff += 1;
//    }
//
//    std::cout << "[puts] param: \"" << Param << "\"" << std::endl;
//
//    u32 RetAddr = 0;
//    if (!pCpuCtxt->ReadRegister(LR, RetAddr))
//      return Emulator::Error;
//    if (!pCpuCtxt->WriteRegister(PC, RetAddr))
//      return Emulator::Error;
//    PutsCalled = true;
//    IsHelloWorld = Param == "hello world!";
//
//    return Emulator::Continue;
//  }));
//
//  bool AbortCalled = false;
//  REQUIRE(Exec.HookFunction("abort", [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const&) -> Emulator::ReturnType
//  {
//    std::cout << "[abort]" << std::endl;
//    AbortCalled = true;
//
//    return Emulator::Break;
//  }));
//
//  Exec.HookInstruction([&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr) -> Emulator::ReturnType
//  {
//    std::cout << pCpuCtxt->ToString() << std::endl;
//    
//    /* TODO(wisk): make a helper for this... */
//    u64 LinAddr;
//    CHECK(pCpuCtxt->Translate(rAddr, LinAddr));
//
//    BinaryStream::SPType spBinStrm;
//    u32 Off;
//    u32 Flags;
//    CHECK(pMemCtxt->FindMemory(LinAddr, spBinStrm, Off, Flags));
//
//    auto ArchTag = pCpuCtxt->GetCpuInformation().GetArchitectureTag();
//    // FIXME(KS): it may not work with thumb mode...
//    auto ArchMode = pCpuCtxt->GetMode();
//    auto& rModMgr = ModuleManager::Instance();
//    auto spArch = rModMgr.GetArchitecture(ArchTag);
//    CHECK(spArch != nullptr);
//
//    auto spCurInsn = std::make_shared<Instruction>();
//    CHECK(spArch->Disassemble(*spBinStrm, Off, *spCurInsn, ArchMode));
//
//    PrintData PD;
//    PD(rAddr);
//    CHECK(spArch->FormatCell(rDoc, rAddr, *spCurInsn, PD));
//    std::cout << PD.GetTexts() << std::endl;
//    for (auto e : spCurInsn->GetSemantic())
//      std::cout << e->ToString() << std::endl;
//
//    return Emulator::Continue;
//  });
//
//  Exec.Execute(StartAddr);
//
//  CHECK(PutsCalled);
//  CHECK(IsHelloWorld);
//  CHECK(AbortCalled);
//
//  Core.CloseDocument();
//}

//TEST_CASE("x86-64", "[emul_interpreter]")
//{
//  using namespace medusa;
//
//  INFO("Using samples path \"" SAMPLES_DIR "\"");
//
//  Medusa Core;
//
//  auto const pSample = SAMPLES_DIR "/exe/hello_world-vs2012.pe.x86-64";
//  std::cout << "emulating program: " << pSample << std::endl;
//
//  try
//  {
//    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
//    REQUIRE(Core.NewDocument(
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
//    REQUIRE(0);
//  }
//
//  Core.WaitForTasks();
//
//  auto& rDoc = Core.GetDocument();
//  auto StartAddr = rDoc.GetAddressFromLabelName("start");
//  auto spStartInsn = std::dynamic_pointer_cast<Instruction>(rDoc.GetCell(StartAddr));
//  auto ArchTag = spStartInsn->GetArchitectureTag();
//  auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
//  REQUIRE(spArch != nullptr);
//  auto Mode = spStartInsn->GetMode();
//  REQUIRE(Mode != 0);
//
//  auto spOs = ModuleManager::Instance().GetOperatingSystem(rDoc.GetOperatingSystemName());
//
//  std::vector<std::string> Args;
//  Args.push_back(pSample);
//  std::vector<std::string> Envp;
//
//  Execution Exec(rDoc, spArch, spOs);
//  REQUIRE(Exec.Initialize(Mode, Args, Envp, SAMPLES_DIR));
//
//  char const* pEmulatorType = "interpreter";
//
//  std::cout << "Using emulator type: " << pEmulatorType << std::endl;
//  REQUIRE(Exec.SetEmulator(pEmulatorType));
//
//  auto pMainCpuCtxt = Exec.GetCpuContext();
//  REQUIRE(pMainCpuCtxt != nullptr);
//
//  u32 RAX = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rax");
//  u32 RCX = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rcx");
//  u32 RDX = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rdx");
//  u32 R8  = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("r8");
//  u32 RSP = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rsp");
//  u32 RIP = pMainCpuCtxt->GetCpuInformation().ConvertNameToIdentifier("rip");
//
//  REQUIRE(RAX != 0 && RCX != 0 && RDX != 0 && R8 != 0 && RSP != 0 && RIP != 0);
//
//  auto Ret = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 StkAddr = 0;
//    u64 RetAddr = 0;
//    REQUIRE(pCpuCtxt->ReadRegister(RSP, StkAddr));
//    REQUIRE(pMemCtxt->ReadMemory(StkAddr, RetAddr));
//
//    std::cout << "[ret] " << Exec.GetHookName() << " called, jumping to " << std::hex << RetAddr << std::endl;
//
//    REQUIRE(pCpuCtxt->WriteRegister(RIP, RetAddr));
//    StkAddr += 8;
//    REQUIRE(pCpuCtxt->WriteRegister(RSP, StkAddr));
//  };
//
//  auto RetValue = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr, u64 Value)
//  {
//    std::cout << "[retval] returning: " << Value << std::endl;
//    u64 OneVal = Value;
//    REQUIRE(pCpuCtxt->WriteRegister(RAX, OneVal));
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
//    REQUIRE(pCpuCtxt->ReadRegister(RCX, Param0));
//    std::cout << "[retparam] returning first parameter: " << std::hex << Param0 << std::endl;
//    REQUIRE(pCpuCtxt->WriteRegister(RAX, Param0));
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
//    REQUIRE(pCpuCtxt->ReadRegister(R8, AllocSize));
//
//    void* pRawMem = nullptr;
//    REQUIRE(pMemCtxt->AllocateMemory(HeapAllocBase, static_cast<u32>(AllocSize), &pRawMem));
//    AllocAddr = HeapAllocBase;
//    HeapAllocBase += AllocSize;
//
//    std::cout << "[alloc_r8] returning allocated buffer: " << std::hex << AllocAddr << ", size: " << AllocSize << std::endl;
//    REQUIRE(pCpuCtxt->WriteRegister(RAX, AllocAddr));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  std::map<u32, u64> TlsMap;
//
//  auto FakeTlsGetValue = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 Param0;
//    REQUIRE(pCpuCtxt->ReadRegister(RCX, Param0));
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
//    REQUIRE(pCpuCtxt->ReadRegister(RCX, Param0));
//    REQUIRE(pCpuCtxt->ReadRegister(RDX, Param1));
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
//    REQUIRE(pCpuCtxt->ReadRegister(RCX, StartupInfoAddr));
//    REQUIRE(pMemCtxt->WriteMemory(StartupInfoAddr, FakeStartupInfoW, sizeof(FakeStartupInfoW)));
//    std::cout << "[GetStartupInfoW] zero out STARTUPINFOW structure: " << std::hex << StartupInfoAddr << std::endl;
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto FakeGetModuleFileNameA = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 FilenameAddr = 0;
//    REQUIRE(pCpuCtxt->ReadRegister(RDX, FilenameAddr));
//    auto FilenameLen = ::strlen(pSample) + 1;
//    REQUIRE(pMemCtxt->WriteMemory(FilenameAddr, pSample, static_cast<u32>(FilenameLen)));
//    std::cout << "[GetModuleFileNameA] write \"" << pSample << "\"" << std::endl;
//
//    RetValue(pCpuCtxt, pMemCtxt, rAddr, FilenameLen - 1);
//  };
//
//  u64 EnvAddr = 0x44440000;
//  auto FakeGetEnvironmentStringsW = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    void* pRawEnv;
//    REQUIRE(pMemCtxt->AllocateMemory(EnvAddr, 2, &pRawEnv));
//    ::memset(pRawEnv, 0x0, 2);
//
//    std::cout << "[GetEnvironmentStringsW] returns empty environ" << std::endl;
//    REQUIRE(pCpuCtxt->WriteRegister(RAX, EnvAddr));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//  auto FakeFreeEnvironmentStringsW = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    void* pRawEnv;
//    REQUIRE(pMemCtxt->AllocateMemory(EnvAddr, 2, &pRawEnv));
//    ::memset(pRawEnv, 0x0, 2);
//
//    std::cout << "[FreeEnvironmentStringsW] free environ" << std::endl;
//    u64 FakeEnvAddr = 0;
//    REQUIRE(pCpuCtxt->ReadRegister(RCX, FakeEnvAddr));
//    REQUIRE(pMemCtxt->FreeMemory(FakeEnvAddr));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto FakeWideCharToMultiByte = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 StkAddr = 0;
//    REQUIRE(pCpuCtxt->ReadRegister(RSP, StkAddr));
//    u64 Param5 = 0;
//    REQUIRE(pMemCtxt->ReadMemory(StkAddr + 0x8 + 0x20, Param5));
//    std::cout << "[WideCharToMultiByte] lpMultiByteStr = " << std::hex << Param5 << std::endl;
//
//    if (Param5 != 0)
//    {
//      std::cout << "[WideCharToMultiByte] write \\x00" << std::endl;
//      REQUIRE(pMemCtxt->WriteMemory(Param5, "\x00", 1));
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
//    REQUIRE(pCpuCtxt->ReadRegister(RCX, Param0));
//
//    void* pAddr;
//    u32 Size;
//    REQUIRE(pMemCtxt->FindMemory(Param0, pAddr, Size));
//    u64 MemSize = Size;
//    REQUIRE(pCpuCtxt->WriteRegister(RAX, MemSize));
//
//    Ret(pCpuCtxt, pMemCtxt, rAddr);
//  };
//
//  auto FakeMessageBoxA = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
//  {
//    u64 Param1 = 0;
//    REQUIRE(pCpuCtxt->ReadRegister(RDX, Param1));
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
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetSystemTimeAsFileTime", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetCurrentThreadId", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetCurrentProcessId", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!QueryPerformanceCounter", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetStartupInfoW", FakeGetStartupInfoW));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetProcessHeap", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!HeapAlloc", AllocR8));
//  REQUIRE(Exec.HookFunction("kernel32.dll!HeapSize", FakeHeapSize));
//  REQUIRE(Exec.HookFunction("kernel32.dll!HeapFree", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!EncodePointer", RetParam0));
//  REQUIRE(Exec.HookFunction("kernel32.dll!DecodePointer", RetParam0));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetModuleHandleW", Ret0));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetModuleHandleExW", Ret0));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetProcAddress", Ret0));
//  REQUIRE(Exec.HookFunction("kernel32.dll!InitializeCriticalSectionAndSpinCount", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!EnterCriticalSection", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!LeaveCriticalSection", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!DeleteCriticalSection", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!Sleep", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!TlsAlloc", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!TlsFree", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!TlsGetValue", FakeTlsGetValue));
//  REQUIRE(Exec.HookFunction("kernel32.dll!TlsSetValue", FakeTlsSetValue));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetLastError", Ret0));
//  REQUIRE(Exec.HookFunction("kernel32.dll!SetLastError", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetStdHandle", Ret));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetFileType", Ret4));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetCommandLineA", Ret0));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetEnvironmentStringsW", FakeGetEnvironmentStringsW));
//  REQUIRE(Exec.HookFunction("kernel32.dll!FreeEnvironmentStringsW", FakeFreeEnvironmentStringsW));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetModuleFileNameA", FakeGetModuleFileNameA));
//  REQUIRE(Exec.HookFunction("kernel32.dll!WideCharToMultiByte", FakeWideCharToMultiByte));
//  REQUIRE(Exec.HookFunction("kernel32.dll!SetUnhandledExceptionFilter", Ret0));
//  REQUIRE(Exec.HookFunction("kernel32.dll!HeapSize", FakeHeapSize));
//  REQUIRE(Exec.HookFunction("kernel32.dll!GetACP", Ret65001));
//
//  REQUIRE(Exec.HookFunction("user32.dll!MessageBoxA", FakeMessageBoxA));
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