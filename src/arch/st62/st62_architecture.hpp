#ifndef ARCH_ST62_HPP
#define ARCH_ST62_HPP

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
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1) const { return false; }
  } m_CpuInfo;

  class St62CpuContext : public CpuContext
  {
  public:
    St62CpuContext(CpuInformation const& rCpuInfo) : CpuContext(rCpuInfo) { memset(&m_Context, 0x0, sizeof(m_Context)); memset(&m_Context.Stk, 0xFF, sizeof(m_Context.Stk));}
    virtual bool ReadRegister (u32 Reg, void*       pVal, u32 BitSize) const;
    virtual bool WriteRegister(u32 Reg, void const* pVal, u32 BitSize);
    virtual bool Translate(Address const& rLogicalAddress, u64& rLinearAddress) const;
    virtual u8 GetMode(void) const;
    virtual void SetMode(u8 Mode);
    virtual bool GetAddress(AddressKind AddrKind, Address& rAddr) const;
    virtual bool SetAddress(AddressKind AddrKind, Address const& rAddr);
    virtual std::string ToString(void) const;

    virtual void* GetRegisterAddress(u32 Register);
    virtual void* GetContextAddress(void);
    virtual u16   GetRegisterOffset(u32 Register);

    virtual void  GetRegisters(RegisterList& RegList) const;

  private:
    struct Context
    {
      u8 A, X, Y, V, W;
      bool ZF, CF;
      u16 Pc;
      u16 Stk[6];
    } m_Context;
  };


  enum ST62Mode
  {
    ST62_Mode_Unknown,
    ST62_Mode_Original
  };

  enum ST62TestedFlag
  {
    ST62_Insn_Prefix_None,
    ST62_Insn_Prefix_NotFlag,
  };

public:
  St62Architecture(void) : Architecture(MEDUSA_ARCH_TAG('S','T','6')) {}

  virtual std::string           GetName(void) const { return "ST62"; }
  virtual bool                  Translate(Address const& rVirtAddr, OffsetType& rPhyslOff) { return false; }
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  virtual NamedModeVector       GetModes(void) const
  {
    NamedModeVector Modes;
    Modes.push_back(NamedMode("ST62x25", ST62_Mode_Original));
    return Modes;
  }
  virtual EEndianness           GetEndianness(void) { return LittleEndian; }
  virtual CpuInformation const* GetCpuInformation(void) const { return &m_CpuInfo; }
  virtual CpuContext*           MakeCpuContext(void) const { return new St62CpuContext(m_CpuInfo); }
  virtual MemoryContext*        MakeMemoryContext(void) const { return new MemoryContext(m_CpuInfo); }
  virtual bool                  EmitSetExecutionAddress(Expression::VSPType& rExprs, Address const& rAddr, u8 Mode);

  virtual bool FormatOperand(
    Document      const& rDoc,
    Address       const& rAddress,
    Instruction   const& rInstruction,
    u8                   OperandNo,
    PrintData          & rPrintData) const;

private:
#include "st62_opcode.ipp"
#include "st62_operand.ipp"

  Expression::SPType __Decode_ext(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_pcr(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_ee(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_direct(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_imm(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_bitdirect(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode);

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
  ST62_Reg_Stk0,
  ST62_Reg_Stk1,
  ST62_Reg_Stk2,
  ST62_Reg_Stk3,
  ST62_Reg_Stk4,
  ST62_Reg_Stk5,
};

#endif // !ARCH_ST62_HPP
