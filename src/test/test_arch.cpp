#define BOOST_TEST_MODULE TestArchitecture
#include <boost/test/unit_test.hpp> 

#include <medusa/binary_stream.hpp>
#include <medusa/module.hpp>
#include <medusa/architecture.hpp>
#include <medusa/instruction.hpp>

BOOST_AUTO_TEST_SUITE(architecture_test_suite)

BOOST_AUTO_TEST_CASE(arch_arm_test_case)
{
  BOOST_MESSAGE("Testing ARM architecture");

  auto& rModMgr = medusa::ModuleManager::Instance();
  medusa::Document Doc;
  medusa::Address Addr;
  auto pArmGetter = rModMgr.LoadModule<medusa::TGetArchitecture>(".", "arm");
  BOOST_REQUIRE(pArmGetter != nullptr);
  auto pArmDisasm = pArmGetter();

  auto const ArmMode = pArmDisasm->GetModeByName("arm");
  auto const ThumbMode = pArmDisasm->GetModeByName("thumb");
  BOOST_REQUIRE(ArmMode != 0);
  BOOST_REQUIRE(ThumbMode != 0);

  {
    // e52de004        push    {lr}            ; (str lr, [sp, #-4]!)
    medusa::MemoryBinaryStream MemBinStrm("\x04\xe0\x2d\xe5", 4);
    medusa::Instruction Insn;
    BOOST_CHECK(pArmDisasm->Disassemble(MemBinStrm, 0x0, Insn, ArmMode));
    medusa::PrintData Data;
    BOOST_CHECK(pArmDisasm->FormatInstruction(Doc, Addr, Insn, Data));
    BOOST_TEST_MESSAGE("0xe52de004 is push {lr} (str lr, [sp, #-4], decoded as: " << Data.GetTexts());
  }

  delete pArmDisasm;
}

BOOST_AUTO_TEST_CASE(arch_avr8_test_case)
{
}

BOOST_AUTO_TEST_CASE(arch_gameboy_test_case)
{
}

BOOST_AUTO_TEST_CASE(arch_x86_test_case)
{
  BOOST_MESSAGE("Testing x86 architecture");

  auto& rModMgr = medusa::ModuleManager::Instance();
  medusa::Document Doc;
  medusa::Address Addr;

  auto pGetDbText = rModMgr.LoadModule<medusa::TGetDatabase>(".", "text");
  BOOST_REQUIRE(pGetDbText != nullptr);
  auto spDbText = medusa::Database::SPType(pGetDbText());

  Doc.Use(spDbText);

  auto pX86Getter = rModMgr.LoadModule<medusa::TGetArchitecture>(".", "x86");
  BOOST_REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_16_Mode = pX86Disasm->GetModeByName("16-bit");
  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  auto const X86_64_Mode = pX86Disasm->GetModeByName("64-bit");
  BOOST_REQUIRE(X86_16_Mode != 0);
  BOOST_REQUIRE(X86_32_Mode != 0);
  BOOST_REQUIRE(X86_64_Mode != 0);

  {
    BOOST_MESSAGE("Testing Ev decoding");

    auto const pAddressingTest =
      "\xFE\x0D\xFA\x0F\x00\x00"     // dec byte  [rel 0x1000]
      "\x66\xFF\x0D\xF3\x0F\x00\x00" // dec word  [rel 0x1000]
      "\xFF\x0D\xED\x0F\x00\x00"     // dec dword [rel 0x1000]
      "\x48\xFF\x0D\xE6\x0F\x00\x00" // dec qword [rel 0x1000]
      ;
    medusa::MemoryBinaryStream MBS(pAddressingTest, 0x6 + 0x7 + 0x6 + 0x7);
    medusa::Instruction InsnDecByteAddr, InsnDecWordAddr, InsnDecDwordAddr, InsnDecQwordAddr;
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0x0, InsnDecByteAddr, X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0x6, InsnDecWordAddr, X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0x6 + 0x7, InsnDecDwordAddr, X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0x6 + 0x7 + 0x6, InsnDecQwordAddr, X86_64_Mode));

    medusa::PrintData PD;
    BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr + 0x0, InsnDecByteAddr, PD));
    PD.AppendNewLine();
    BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr + 0x6, InsnDecWordAddr, PD));
    PD.AppendNewLine();
    BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr + 0x6 + 0x7, InsnDecDwordAddr, PD));
    PD.AppendNewLine();
    BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr + 0x6 + 0x7 + 0x6, InsnDecQwordAddr, PD));
    PD.AppendNewLine();

    std::cout << InsnDecByteAddr.ToString() << std::endl;
    std::cout << InsnDecWordAddr.ToString() << std::endl;
    std::cout << InsnDecDwordAddr.ToString() << std::endl;
    std::cout << InsnDecQwordAddr.ToString() << std::endl;
    std::cout << PD.GetTexts() << std::endl;
  }

  {
    BOOST_MESSAGE("Testing Ev, Gv decoding");

    auto const pAddressingOperandTest =
      "\x67\x88\x00"     // mov byte  [eax], al
      "\x66\x67\x89\x00" // mov word  [eax], ax
      "\x67\x89\x00"     // mov dword [eax], eax
      "\x67\x48\x89\x00" // mov qword [eax], rax
      "\x88\x00"         // mov byte  [rax], al
      "\x66\x89\x00"     // mov word  [rax], ax
      "\x89\x00"         // mov dword [rax], eax
      "\x48\x89\x00"     // mov qword [rax], rax
      ;

    medusa::MemoryBinaryStream MBS(pAddressingOperandTest, 3 + 4 + 3 + 4 + 2 + 3 + 2 + 3);
    medusa::Instruction InsnArr[8];
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0,  InsnArr[0], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 3,  InsnArr[1], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7,  InsnArr[2], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 10, InsnArr[3], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 14, InsnArr[4], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 16, InsnArr[5], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 19, InsnArr[6], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 21, InsnArr[7], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    BOOST_MESSAGE("Testing Ev, Iz decoding");

    auto const pAddressingImmediate =
      "\xC6\x00\xCC"                 // mov byte [rax], 0xcc
      "\x66\xC7\x00\xCC\xCC"         // mov word [rax], 0xcccc
      "\xC7\x00\xCC\xCC\xCC\xCC"     // mov dword [rax], 0xcccccccc
      "\x48\xC7\x00\xCC\xCC\xCC\xCC" // mov qword [rax], 0xcccccccc
      "\x83\xec\x10"                 // sub esp, 0x00000010
      ;

    medusa::MemoryBinaryStream MBS(pAddressingImmediate, 3 + 5 + 6 + 7 + 3);
    medusa::Instruction InsnArr[5];
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0,  InsnArr[0], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 3,  InsnArr[1], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 8,  InsnArr[2], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 14, InsnArr[3], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 21, InsnArr[4], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    BOOST_MESSAGE("Testing SIB");

    auto const pSib =
      "\x88\x8C\xD8\x90\x90\x90\x90" // mov [rax + rbx * 8 + 0x90909090], cl
      ;

    medusa::MemoryBinaryStream MBS(pSib, 7);
    medusa::Instruction Insn;
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0, Insn, X86_64_Mode));

    std::cout << Insn.ToString() << std::endl;
    medusa::PrintData PD;
    BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr, Insn, PD));
    std::cout << PD.GetTexts() << std::endl;
  }

  {
    BOOST_MESSAGE("Testing Jb/Jv decoding");

    auto const pRelative =
      "\xEB\x90"             // jmp short $+0x90
      "\x66\xE9\x90\x90"     // jmp word  $+0x9090
      "\xE9\x90\x90\x90\x90" // jmp dword $+0x90909090
      ;

    medusa::MemoryBinaryStream MBS(pRelative, 2 + 4 + 5);
    medusa::Instruction InsnArr[3];
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0, InsnArr[0], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 2, InsnArr[1], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 6, InsnArr[2], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    auto const pSimd =
      "\x0F\x6E\xC0"                         // movd mm0,  eax
      "\x48\x0F\x6E\xC0"                     // movq mm0,  rax
      "\x66\x48\x0F\x6E\xC0"                 // movq xmm0, rax
      "\x0F\x6F\x0C\x25\x44\x33\x22\x11"     // movq mm1,  [0x11223344]
      "\xF3\x0F\x7E\x0C\x25\x44\x33\x22\x11" // movq xmm1, [0x11223344]
      ;

    medusa::MemoryBinaryStream MBS(pSimd, 3 + 4 + 5 + 8 + 9);
    medusa::Instruction InsnArr[5];
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0, InsnArr[0], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 3, InsnArr[1], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7, InsnArr[2], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 12, InsnArr[3], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 20, InsnArr[4], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    auto const pBuggyInsn =
      "\x83\x3D\xD4\x40\xD2\x4A\x00" // cmp dword [dword 0x4ad240d4],byte +0x0
      "\xFF\x15\xD4\x40\xD2\x00"     // call dword [0xd240d4]
      "\x8D\x6C\x24\x90"             // lea ebp, dword [esp-0x70]
      "\x8D\x04\x0A"                 // lea eax, dword [edx+ecx]
      "\x8B\x34\x85\xC0\x60\x41\x00" // mov esi, dword [rax * 4 + 0x4160c0]
      "\xFF\x24\xC5\xF0\x42\x41\x00" // jmp qword [rax*8+0x4142f0]
      "\x48\x89\x5C\x24\x20"         // mov qword [rsp+20h], rbx
      "\x64\x44\x8B\x24\x25\xF8\xFF\xFF\xFF" // mov r12d, dword fs:[0xFFFFFFFFFFFFFFF8]
      "\xff\x24\xfd\x10\x06\x40\x00" // jmp qword [rdi * 8 + 0x400610]
      ;

    medusa::MemoryBinaryStream MBS(pBuggyInsn, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9 + 7);
    medusa::Instruction InsnArr[9];
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0, InsnArr[0], X86_32_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7, InsnArr[1], X86_32_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7 + 6, InsnArr[2], X86_32_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4, InsnArr[3], X86_32_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3, InsnArr[4], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7, InsnArr[5], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7, InsnArr[6], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5, InsnArr[7], X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9, InsnArr[8], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    auto const pComplexInsn =
      "\xF0\x0F\xC7\x0E" // lock cmpxchg8b qword [esi]
      ;

    medusa::MemoryBinaryStream MBS(pComplexInsn, 4);
    medusa::Instruction InsnArr[1];
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0, InsnArr[0], X86_32_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      BOOST_CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  delete pX86Disasm;
}

BOOST_AUTO_TEST_SUITE_END()
