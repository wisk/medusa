#ifndef ARCH_LR35902_HPP
#define ARCH_LR35902_HPP

#include "medusa/types.hpp"
#include "medusa/extend.hpp"
#include "medusa/architecture.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/instruction.hpp"
#include "medusa/context.hpp"

#if defined(_WIN32) || defined(WIN32)
# ifdef arch_lr35902_EXPORTS
#  define ARCH_LR35902_EXPORT __declspec(dllexport)
# else
#  define ARCH_LR35902_EXPORT __declspec(dllimport)
# endif
#else
# define ARCH_LR35902_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" ARCH_LR35902_EXPORT Architecture* GetArchitecture(void);

class LR35902Architecture : public Architecture
{
private:
  class LR35902CpuInformation : public CpuInformation
  {
  public:
    LR35902CpuInformation(void) : CpuInformation(MEDUSA_ARCH_TAG('l', 'r', '3')) {}
    virtual char const* ConvertIdentifierToName(u32 Id) const;
    virtual u32 ConvertNameToIdentifier(std::string const& rName) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id) const;
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1) const;
  } m_CpuInfo;

  enum LR35902Mode
  {
    LR35902_Mode_Default = 1,
  };

  enum LR35902InsnPrefix
  {
    LR35902_Insn_Prefix_None,
    LR35902_Insn_Prefix_NotFlag,
  };

public:
  LR35902Architecture(void) : Architecture(MEDUSA_ARCH_TAG('l','r','3')) {}

  virtual std::string           GetName(void) const { return "Sharp LR35902"; }
  virtual bool                  Translate(Address const& rVirtAddr, OffsetType& rPhyslOff);
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
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
};

enum LR35902Identifiers
{
  LR35902_Reg_Unknown,

  LR35902_Flg_C,
  LR35902_Flg_H,
  LR35902_Flg_N,
  LR35902_Flg_Z,

  LR35902_Reg_A,
  LR35902_Reg_B,
  LR35902_Reg_C,
  LR35902_Reg_D,
  LR35902_Reg_E,
  LR35902_Reg_F,
  LR35902_Reg_H,
  LR35902_Reg_L,
  LR35902_Reg_AF,
  LR35902_Reg_BC,
  LR35902_Reg_DE,
  LR35902_Reg_HL,
  LR35902_Reg_Sp,
  LR35902_Reg_Pc,
};

#endif // !ARCH_LR35902_HPP
