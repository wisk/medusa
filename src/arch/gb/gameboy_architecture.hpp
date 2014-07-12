#ifndef _GAMEBOY_ARCHITECTURE_
#define _GAMEBOY_ARCHITECTURE_

#include "medusa/types.hpp"
#include "medusa/architecture.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/instruction.hpp"
#include "medusa/context.hpp"

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
private:
  class GameBoyCpuInformation : public CpuInformation
  {
  public:
    virtual char const* ConvertIdentifierToName(u32 Id) const;
    virtual u32 ConvertNameToIdentifier(std::string const& rName) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id) const;
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1) const;
  } m_CpuInfo;

public:
  GameBoyArchitecture(void) : Architecture(MEDUSA_ARCH_TAG('n','g','b')) {}

  virtual std::string           GetName(void) const { return "Nintendo GameBoy Z80"; }
  virtual bool                  Translate(Address const& rVirtAddr, TOffset& rPhyslOff);
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  virtual NamedModeVector       GetModes(void) const
  {
    NamedModeVector GbModes;
    GbModes.reserve(1);
    GbModes.push_back(NamedMode("gameboy", 0));
    return GbModes;
  }
  virtual EEndianness           GetEndianness(void) { return LittleEndian; }
  virtual CpuInformation const* GetCpuInformation(void) const { return &m_CpuInfo; }
  virtual CpuContext*           MakeCpuContext(void) const { return nullptr; }
  virtual MemoryContext*        MakeMemoryContext(void) const { return new MemoryContext(m_CpuInfo); }

private:
  typedef bool (GameBoyArchitecture:: *TDisassembler)(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);

  typedef struct  SRegName
  {
    u16           m_Value;
    char const*   m_Name;
  }               TRegName;

  static TDisassembler const m_OpcodeMap[0x100];
  static TDisassembler const m_CbPrefix [0x100];
  static TRegName      const m_RegName  [];

  u16  GetRegisterByOpcode(u8 Opcode);

  bool Insn_Invalid(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Nop(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);

  bool Insn_Inc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Dec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Add(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Sub(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Adc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Sbc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Daa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);

  bool Insn_And(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Or(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Xor(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Bit(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Set(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Res(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Rl(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Rr(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Rlc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Rrc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Sla(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Sra(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Srl(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Swap(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Cpl(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Ccf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Scf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Cp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);

  bool Insn_Ld(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Ldi(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Ldd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Ldh(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Ldhl(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Push(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Pop(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);

  bool Insn_Jr(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Jp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Call(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Rst(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Ret(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Reti(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);

  bool Insn_Halt(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Stop(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Ei(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  bool Insn_Di(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
};

#endif // _GAMEBOY_ARCHITECTURE_