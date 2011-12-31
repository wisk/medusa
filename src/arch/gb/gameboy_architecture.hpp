#ifndef _GAMEBOY_ARCHITECTURE_
#define _GAMEBOY_ARCHITECTURE_

#include "medusa/types.hpp"
#include "medusa/architecture.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/instruction.hpp"

#include "gameboy_instruction.hpp"
#include "gameboy_register.hpp"

#if defined(_WIN32) || defined(WIN32)
# ifdef arch_gb_EXPORTS
#  define ARCH_GB_EXPORT __declspec(dllexport)
# else
#  define ARCH_GB_EXPORT __declspec(dllimport)
# endif
#else
# define ARCH_GB_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" ARCH_GB_EXPORT Architecture* GetArchitecture(void);

class GameBoyArchitecture : public Architecture
{
public:
  GameBoyArchitecture(void) {}

  virtual std::string GetName(void) { return "Nintendo GameBoy Z80"; }
  virtual bool        Translate(Address const& rVirtAddr, TAddress& rPhyslAddr);
  virtual bool        Disassemble(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  virtual void        FillConfigurationModel(ConfigurationModel& rCfgMdl);

  virtual EEndianness GetEndianness(void) { return LittleEndian; }

private:
  typedef bool (GameBoyArchitecture:: *TDisassembler)(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);

  typedef struct  SRegName
  {
    u16           m_Value;
    char const*   m_Name;
  }               TRegName;

  static TDisassembler const m_OpcodeMap[0x100];
  static TDisassembler const m_CbPrefix [0x100];
  static TRegName      const m_RegName  [];

  u16  GetRegisterByOpcode(u8 Opcode);
  void FormatOperand(Operand& Op, TAddress Address);

  bool Insn_Invalid(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Nop(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);

  bool Insn_Inc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Dec(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Add(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sub(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Adc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Daa(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);

  bool Insn_And(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Or(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Xor(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Bit(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Set(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Res(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Rl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Rr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Rlc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Rrc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sla(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sra(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Srl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Swap(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cpl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ccf(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Scf(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);

  bool Insn_Ld(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ldi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ldd(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ldh(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ldhl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Push(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Pop(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);

  bool Insn_Jr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Jp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Call(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Rst(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ret(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Reti(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);

  bool Insn_Halt(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Stop(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ei(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Di(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
};

#endif // _GAMEBOY_ARCHITECTURE_
