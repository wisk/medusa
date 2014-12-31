#ifndef ARCH_GAMEBOY_HPP
#define ARCH_GAMEBOY_HPP

#include "medusa/types.hpp"
#include "medusa/architecture.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/instruction.hpp"
#include "medusa/context.hpp"

#if defined(_WIN32) || defined(WIN32)
# ifdef arch_z80_EXPORTS
#  define ARCH_Z80_EXPORT __declspec(dllexport)
# else
#  define ARCH_Z80_EXPORT __declspec(dllimport)
# endif
#else
# define ARCH_Z80_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" ARCH_Z80_EXPORT Architecture* GetArchitecture(void);

// TODO: is gameboy CPU really a Z80? http://www.assemblergames.com/forums/archive/index.php/t-20518.html
class Z80Architecture : public Architecture
{
private:
  class Z80CpuInformation : public CpuInformation
  {
  public:
    virtual char const* ConvertIdentifierToName(u32 Id) const;
    virtual u32 ConvertNameToIdentifier(std::string const& rName) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id) const;
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1) const;
  } m_CpuInfo;

  enum Z80Mode
  {
    Z80_Mode_Unknown,
    Z80_Mode_Original,
    Z80_Mode_LR35902,
  };

  enum Z80TestedFlag
  {
    Z80_Insn_Prefix_None,
    Z80_Insn_Prefix_NotFlag,
  };

public:
  Z80Architecture(void) : Architecture(MEDUSA_ARCH_TAG('z','8','0')) {}

  virtual std::string           GetName(void) const { return "Zilog 80"; }
  virtual bool                  Translate(Address const& rVirtAddr, TOffset& rPhyslOff);
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  virtual NamedModeVector       GetModes(void) const;
  virtual EEndianness           GetEndianness(void) { return LittleEndian; }
  virtual CpuInformation const* GetCpuInformation(void) const { return &m_CpuInfo; }
  virtual CpuContext*           MakeCpuContext(void) const { return nullptr; }
  virtual MemoryContext*        MakeMemoryContext(void) const { return new MemoryContext(m_CpuInfo); }

  virtual bool FormatInstruction(
    Document      const& rDoc,
    Address       const& rAddr,
    Instruction   const& rInsn,
    PrintData          & rPrintData) const;

private:
};

enum Z80Identifiers
{
  Z80_Reg_Unknown,

  Z80_Flg_C,
  Z80_Flg_H,
  Z80_Flg_N,
  Z80_Flg_Z,

  Z80_Reg_A,
  Z80_Reg_B,
  Z80_Reg_C,
  Z80_Reg_D,
  Z80_Reg_E,
  Z80_Reg_F,
  Z80_Reg_H,
  Z80_Reg_L,
  Z80_Reg_AF,
  Z80_Reg_BC,
  Z80_Reg_DE,
  Z80_Reg_HL,
  Z80_Reg_Sp,
  Z80_Reg_Pc,
};

#endif // !ARCH_GAMEBOY_HPP