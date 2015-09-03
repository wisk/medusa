#ifndef ARCH_GAMEBOY_HPP
#define ARCH_GAMEBOY_HPP

#include "medusa/types.hpp"
#include "medusa/extend.hpp"
#include "medusa/architecture.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/instruction.hpp"
#include "medusa/context.hpp"

#if defined(_WIN32) || defined(WIN32)
# ifdef arch_st62_EXPORTS
#  define ARCH_ST62_EXPORT __declspec(dllexport)
# else
#  define ARCH_ST62_EXPORT __declspec(dllimport)
# endif
#else
# define ARCH_ST62_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" ARCH_ST62_EXPORT Architecture* GetArchitecture(void);

class St62Architecture : public Architecture
{
private:
  class ST62CpuInformation : public CpuInformation
  {
  public:
    ST62CpuInformation(void) : CpuInformation(MEDUSA_ARCH_TAG('S', 'T', '6')) {}
    virtual char const* ConvertIdentifierToName(u32 Id) const;
    virtual u32 ConvertNameToIdentifier(std::string const& rName) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id) const;
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1) const;
  } m_CpuInfo;

  enum ST62Mode
  {
    ST62_Mode_Unknown,
    ST62_Mode_Original,
    ST62_Mode_LR35902,
  };

  enum ST62TestedFlag
  {
    ST62_Insn_Prefix_None,
    ST62_Insn_Prefix_NotFlag,
  };

public:
  St62Architecture(void) : Architecture(MEDUSA_ARCH_TAG('S','T','6')) {}

  virtual std::string           GetName(void) const { return "ST62"; }
  virtual bool                  Translate(Address const& rVirtAddr, TOffset& rPhyslOff) { return false; }
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  virtual NamedModeVector       GetModes(void) const
  {
    NamedModeVector Modes;
    Modes.push_back(NamedMode("ST62x25", ST62_Mode_Original));
    return Modes;
  }
  virtual EEndianness           GetEndianness(void) { return LittleEndian; }
  virtual CpuInformation const* GetCpuInformation(void) const { return &m_CpuInfo; }
  virtual CpuContext*           MakeCpuContext(void) const { return nullptr; }
  virtual MemoryContext*        MakeMemoryContext(void) const { return new MemoryContext(m_CpuInfo); }

  virtual bool FormatOperand(
    Document      const& rDoc,
    Address       const& rAddress,
    Instruction   const& rInstruction,
    u8                   OperandNo,
    PrintData          & rPrintData) const;

  virtual bool FormatInstruction(
    Document      const& rDoc,
    Address       const& rAddr,
    Instruction   const& rInsn,
    PrintData          & rPrintData) const;

private:
#include "st62_opcode.ipp"
#include "st62_operand.ipp"

  Expression::SPType __Decode_ext(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_pcr(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_ee(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_direct(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_imm(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_bitdirect(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);

private:
  static const char * m_Mnemonic[];
};

enum ST62Identifiers
{
  ST62_Reg_Unknown,

  ST62_Flg_C,
  ST62_Flg_Z,

  ST62_Reg_A,
  ST62_Reg_X,
  ST62_Reg_Y,
  ST62_Reg_V,
  ST62_Reg_W,
  ST62_Reg_Pc,
};

#endif // !ARCH_GAMEBOY_HPP
