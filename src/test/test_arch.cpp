#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <medusa/binary_stream.hpp>
#include <medusa/module.hpp>
#include <medusa/architecture.hpp>
#include <medusa/instruction.hpp>

TEST_CASE("disassemble", "[arch_arm]")
{
  INFO("Testing ARM architecture");

  auto& rModMgr = medusa::ModuleManager::Instance();
  medusa::Document Doc;
  medusa::Address Addr;
  auto pArmGetter = rModMgr.LoadModule<medusa::TGetArchitecture>(".", "arm");
  REQUIRE(pArmGetter != nullptr);
  auto pArmDisasm = pArmGetter();

  auto const ArmMode = pArmDisasm->GetModeByName("arm");
  auto const ThumbMode = pArmDisasm->GetModeByName("thumb");
  REQUIRE(ArmMode != 0);
  REQUIRE(ThumbMode != 0);

  {
    // e52de004        push    {lr}            ; (str lr, [sp, #-4]!)
    medusa::MemoryBinaryStream MemBinStrm("\x04\xe0\x2d\xe5", 4);
    medusa::Instruction Insn;
    CHECK(pArmDisasm->Disassemble(MemBinStrm, 0x0, Insn, ArmMode));
    medusa::PrintData Data;
    CHECK(pArmDisasm->FormatInstruction(Doc, Addr, Insn, Data));
    INFO("0xe52de004 is push {lr} (str lr, [sp, #-4], decoded as: " << Data.GetTexts());
  }

  {
    // e710019f ldr r1, [pc,r1]
    medusa::MemoryBinaryStream MemBinStrm("\x01\x10\x9f\xe7", 4);
    medusa::Instruction Insn;
    CHECK(pArmDisasm->Disassemble(MemBinStrm, 0x0, Insn, ArmMode));
    medusa::PrintData Data;
    CHECK(pArmDisasm->FormatInstruction(Doc, Addr, Insn, Data));
    std::cout << Data.GetTexts() << std::endl;
    INFO("e710019f is ldr r1, [pc,r1], decoded as: " << Data.GetTexts());
  }

  delete pArmDisasm;
}

TEST_CASE("disassemble", "[arch_x86]")
{
  INFO("Testing x86 architecture");

  auto& rModMgr = medusa::ModuleManager::Instance();
  medusa::Document Doc;
  medusa::Address Addr;

  auto pGetDbText = rModMgr.LoadModule<medusa::TGetDatabase>(".", "text");
  REQUIRE(pGetDbText != nullptr);
  auto spDbText = medusa::Database::SPType(pGetDbText());

  Doc.Open(spDbText);

  auto pX86Getter = rModMgr.LoadModule<medusa::TGetArchitecture>(".", "x86");
  REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_16_Mode = pX86Disasm->GetModeByName("16-bit");
  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  auto const X86_64_Mode = pX86Disasm->GetModeByName("64-bit");
  REQUIRE(X86_16_Mode != 0);
  REQUIRE(X86_32_Mode != 0);
  REQUIRE(X86_64_Mode != 0);

  {
    INFO("Testing Ev decoding");

    auto const pAddressingTest =
      "\xFE\x0D\xFA\x0F\x00\x00"     // dec byte  [rel 0x1000]
      "\x66\xFF\x0D\xF3\x0F\x00\x00" // dec word  [rel 0x1000]
      "\xFF\x0D\xED\x0F\x00\x00"     // dec dword [rel 0x1000]
      "\x48\xFF\x0D\xE6\x0F\x00\x00" // dec qword [rel 0x1000]
      ;
    medusa::MemoryBinaryStream MBS(pAddressingTest, 0x6 + 0x7 + 0x6 + 0x7);
    medusa::Instruction InsnDecByteAddr, InsnDecWordAddr, InsnDecDwordAddr, InsnDecQwordAddr;
    CHECK(pX86Disasm->Disassemble(MBS, 0x0, InsnDecByteAddr, X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 0x6, InsnDecWordAddr, X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 0x6 + 0x7, InsnDecDwordAddr, X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 0x6 + 0x7 + 0x6, InsnDecQwordAddr, X86_64_Mode));

    medusa::PrintData PD;
    CHECK(pX86Disasm->FormatInstruction(Doc, Addr + 0x0, InsnDecByteAddr, PD));
    PD.AppendNewLine();
    CHECK(pX86Disasm->FormatInstruction(Doc, Addr + 0x6, InsnDecWordAddr, PD));
    PD.AppendNewLine();
    CHECK(pX86Disasm->FormatInstruction(Doc, Addr + 0x6 + 0x7, InsnDecDwordAddr, PD));
    PD.AppendNewLine();
    CHECK(pX86Disasm->FormatInstruction(Doc, Addr + 0x6 + 0x7 + 0x6, InsnDecQwordAddr, PD));
    PD.AppendNewLine();

    std::cout << InsnDecByteAddr.ToString() << std::endl;
    std::cout << InsnDecWordAddr.ToString() << std::endl;
    std::cout << InsnDecDwordAddr.ToString() << std::endl;
    std::cout << InsnDecQwordAddr.ToString() << std::endl;
    std::cout << PD.GetTexts() << std::endl;
  }

  {
    INFO("Testing Ev, Gv decoding");

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
    CHECK(pX86Disasm->Disassemble(MBS, 0,  InsnArr[0], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 3,  InsnArr[1], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7,  InsnArr[2], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 10, InsnArr[3], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 14, InsnArr[4], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 16, InsnArr[5], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 19, InsnArr[6], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 21, InsnArr[7], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    INFO("Testing Ev, Iz decoding");

    auto const pAddressingImmediate =
      "\xC6\x00\xCC"                 // mov byte [rax], 0xcc
      "\x66\xC7\x00\xCC\xCC"         // mov word [rax], 0xcccc
      "\xC7\x00\xCC\xCC\xCC\xCC"     // mov dword [rax], 0xcccccccc
      "\x48\xC7\x00\xCC\xCC\xCC\xCC" // mov qword [rax], 0xcccccccc
      "\x83\xec\x10"                 // sub esp, 0x00000010
      ;

    medusa::MemoryBinaryStream MBS(pAddressingImmediate, 3 + 5 + 6 + 7 + 3);
    medusa::Instruction InsnArr[5];
    CHECK(pX86Disasm->Disassemble(MBS, 0,  InsnArr[0], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 3,  InsnArr[1], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 8,  InsnArr[2], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 14, InsnArr[3], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 21, InsnArr[4], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    INFO("Testing SIB");

    auto const pSib =
      "\x88\x8C\xD8\x90\x90\x90\x90" // mov [rax + rbx * 8 + 0x90909090], cl
      ;

    medusa::MemoryBinaryStream MBS(pSib, 7);
    medusa::Instruction Insn;
    CHECK(pX86Disasm->Disassemble(MBS, 0, Insn, X86_64_Mode));

    std::cout << Insn.ToString() << std::endl;
    medusa::PrintData PD;
    CHECK(pX86Disasm->FormatInstruction(Doc, Addr, Insn, PD));
    std::cout << PD.GetTexts() << std::endl;
  }

  {
    INFO("Testing Jb/Jv decoding");

    auto const pRelative =
      "\xEB\x90"             // jmp short $+0x90
      "\x66\xE9\x90\x90"     // jmp word  $+0x9090
      "\xE9\x90\x90\x90\x90" // jmp dword $+0x90909090
      ;

    medusa::MemoryBinaryStream MBS(pRelative, 2 + 4 + 5);
    medusa::Instruction InsnArr[3];
    CHECK(pX86Disasm->Disassemble(MBS, 0, InsnArr[0], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 2, InsnArr[1], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 6, InsnArr[2], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
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
    CHECK(pX86Disasm->Disassemble(MBS, 0, InsnArr[0], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 3, InsnArr[1], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7, InsnArr[2], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 12, InsnArr[3], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 20, InsnArr[4], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
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
      "\x66\x41\x81\xc3\xaa\xbb"     // add r11w, 0xbbaa
      "\x41\x52"                     // push r10
      "\x98"                         // cwde
      "\x66\x98"                     // cbw
      "\x41\x83\x65\x00\x00"         // 
      ;

    medusa::MemoryBinaryStream MBS(pBuggyInsn, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9 + 7 + 6 + 2 + 1 + 2 + 5);
    medusa::Instruction InsnArr[14];
    CHECK(pX86Disasm->Disassemble(MBS, 0, InsnArr[0], X86_32_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7, InsnArr[1], X86_32_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6, InsnArr[2], X86_32_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4, InsnArr[3], X86_32_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3, InsnArr[4], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7, InsnArr[5], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7, InsnArr[6], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5, InsnArr[7], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9, InsnArr[8], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9 + 7, InsnArr[9], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9 + 7 + 6, InsnArr[10], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9 + 7 + 6 + 2, InsnArr[11], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9 + 7 + 6 + 2 + 1, InsnArr[12], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 7 + 6 + 4 + 3 + 7 + 7 + 5 + 9 + 7 + 6 + 2 + 1 + 2, InsnArr[13], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    auto const pComplexInsn =
      "\xF0\x0F\xC7\x0E" // lock cmpxchg8b qword [esi]
      ;

    medusa::MemoryBinaryStream MBS(pComplexInsn, 4);
    medusa::Instruction InsnArr[1];
    CHECK(pX86Disasm->Disassemble(MBS, 0, InsnArr[0], X86_32_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  {
    auto const pSimdInsn =
      "\xF3\x0F\x58\x05\xB8\x2A\x0B\x08" // addss xmm0, oword ds:[xxx]
      "\xF3\x0F\x10\x35\xBC\x2A\x0B\x08" // movss xmm6, oword ds:[xxx]
      "\x66\x0F\x73\xD1\x01"             // psrlq xmm1, 1
      "\xF3\x0F\x10\xD1"                 // movss   xmm2, xmm1
      "\x66\x0F\x70\xDF\xD4"             // pshufd xmm3, xmm1, 0xd4
      ;

    medusa::MemoryBinaryStream MBS(pSimdInsn, 8 + 8 + 5 + 4 + 5);
    medusa::Instruction InsnArr[5];
    CHECK(pX86Disasm->Disassemble(MBS,  0, InsnArr[0], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS,  8, InsnArr[1], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 16, InsnArr[2], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 21, InsnArr[3], X86_64_Mode));
    CHECK(pX86Disasm->Disassemble(MBS, 25, InsnArr[4], X86_64_Mode));

    for (auto const& rInsn : InsnArr)
    {
      std::cout << rInsn.ToString() << std::endl;
      medusa::PrintData PD;
      CHECK(pX86Disasm->FormatInstruction(Doc, Addr, rInsn, PD));
      std::cout << PD.GetTexts() << std::endl;
    }
  }

  delete pX86Disasm;
}

TEST_CASE("disassemble", "[arch_st62]")
{
  INFO("Testing ST62 architecture");

  auto& rModMgr = medusa::ModuleManager::Instance();
  medusa::Document Doc;
  medusa::Address Addr;
  auto pSt62Getter = rModMgr.LoadModule<medusa::TGetArchitecture>(".", "st62");
  REQUIRE(pSt62Getter != nullptr);
  auto pSt62Disasm = pSt62Getter();

  auto const St62x25Mode = pSt62Disasm->GetModeByName("ST62x25");
  REQUIRE(St62x25Mode != 0);

  {
    // 0000:0000000000000000  jp               0xC1
    medusa::MemoryBinaryStream MemBinStrm("\x19\x0c", 2);
    medusa::Instruction Insn;
    CHECK(pSt62Disasm->Disassemble(MemBinStrm, 0x0, Insn, St62x25Mode));
    medusa::PrintData Data;
    CHECK(pSt62Disasm->FormatInstruction(Doc, Addr, Insn, Data));
    INFO("jp 0xC1, decoded as: " << Data.GetTexts());
    CHECK(Data.GetTexts() == "0000:0000000000000000  jp              0xC1");
  }

  delete pSt62Disasm;
}