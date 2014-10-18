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
    auto const pAddressingTest =
      "\xFE\x0D\xFA\x0F\x00\x00" // dec byte [rel 0x1000]
      "\x66\xFF\x0D\xF3\x0F\x00\x00" // dec word [rel 0x1000]
      "\xFF\x0D\xED\x0F\x00\x00" // dec dword [rel 0x1000]
      "\x48\xFF\x0D\xE6\x0F\x00\x00" // dec qword [rel 0x1000]
      ;
    medusa::MemoryBinaryStream MBS(pAddressingTest, 0x6 + 0x7 + 0x6 + 0x7);
    medusa::Instruction InsnDecByteAddr, InsnDecWordAddr, InsnDecDwordAddr, InsnDecQwordAddr;
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0x0, InsnDecByteAddr, X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0x6, InsnDecWordAddr, X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0x6 + 0x7, InsnDecDwordAddr, X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(MBS, 0x6 + 0x7 + 0x6, InsnDecQwordAddr, X86_64_Mode));

    std::cout << InsnDecByteAddr.GetOperand(0)->ToString() << std::endl;
    std::cout << InsnDecWordAddr.GetOperand(0)->ToString() << std::endl;
    std::cout << InsnDecDwordAddr.GetOperand(0)->ToString() << std::endl;
    std::cout << InsnDecQwordAddr.GetOperand(0)->ToString() << std::endl;
  }

  delete pX86Disasm;
}

BOOST_AUTO_TEST_SUITE_END()
