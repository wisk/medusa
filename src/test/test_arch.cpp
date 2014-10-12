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
    medusa::MemoryBinaryStream Test0("\xFF\x0D\xFA\x0F\x00\x00", 6); // dec dword [rel 0x1000]
    medusa::MemoryBinaryStream Test1("\x48\xFF\x0D\xF3\x0F\x00\x00", 7); // dec qword [rel 0x1000]
    medusa::Instruction Insn0;
    medusa::Instruction Insn1;
    BOOST_CHECK(pX86Disasm->Disassemble(Test0, 0x0, Insn0, X86_64_Mode));
    BOOST_CHECK(pX86Disasm->Disassemble(Test1, 0x0, Insn1, X86_64_Mode));
  }

  delete pX86Disasm;
}

BOOST_AUTO_TEST_SUITE_END()
