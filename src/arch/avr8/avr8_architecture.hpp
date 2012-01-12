#ifndef _AVR8_ARCHITECTURE_
#define _AVR8_ARCHITECTURE_

#include "medusa/types.hpp"
#include "medusa/architecture.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/instruction.hpp"

#include "avr8_instruction.hpp"
#include "avr8_register.hpp"

#if defined(_WIN32) || defined(WIN32)
# ifdef arch_avr8_EXPORTS
#  define ARCH_AVR8_EXPORT __declspec(dllexport)
# else
#  define ARCH_AVR8_EXPORT __declspec(dllimport)
# endif
#else
# define ARCH_AVR8_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" ARCH_AVR8_EXPORT Architecture* GetArchitecture(void);

class Avr8Architecture : public Architecture
{
public:
  Avr8Architecture(void) {}

  virtual std::string GetName(void) { return "Atmel AVR 8-bit"; }
  virtual bool        Translate(Address const& rVirtAddr, TAddress& rPhyslAddr);
  virtual bool        Disassemble(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  virtual void        FillConfigurationModel(ConfigurationModel& rCfgMdl) { }
  virtual EEndianness GetEndianness(void) { return LittleEndian; }

private:
  static char const *m_RegName[];

  void FormatOperand(Operand& Op, TAddress Address);

  bool Insn_(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn); // DELETE WHEN FINISH

  // Handles one instruction
  bool Insn_Invalid(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Adc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Add(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Adiw(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_And(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Andi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Asr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Bclr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Bld(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brbc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brbs(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brcc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brcs(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Break(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Breq(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brge(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brhc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brhs(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brid(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brie(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brlo(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brlt(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brmi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brne(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brpl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brsh(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brtc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brts(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brvc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Brvs(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Bset(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Bst(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Call(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cbi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cbr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Clc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Clh(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cli(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cln(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Clr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cls(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Clt(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Clv(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Clz(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Com(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cpc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cpi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Cpse(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Dec(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Des(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Eicall(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Eijmp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Elpm(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Eor(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Fmul(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Fmuls(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Fmulsu(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Icall(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ijmp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_In(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Inc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Jmp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ldi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Lds(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ld(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Lpm(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Lsl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Lsr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Mov(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Movw(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Muls(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Mulsu(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Mulu(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Neg(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Nop(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Or(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ori(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Out(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Pop(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Push(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Rcall(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ret(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Reti(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Rjmp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Rol(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ror(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbci(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbic(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbis(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbiw(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbrc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sbrs(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sec(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Seh(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sei(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sen(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ser(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Ses(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Set(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sev(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sez(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sleep(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Spm(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sts(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_St(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Sub(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Subi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Swap(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Tst(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_Wdr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);

  // Handles multiple instructions
  bool Insn_0xxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_1xxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_2xxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_3xxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_8xxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_9xxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_axxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_90xx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_92xx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_94xx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_96xx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_98xx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_bxxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
  bool Insn_fxxx(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn);
};

#endif
