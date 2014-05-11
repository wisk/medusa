#ifndef _X86_ARCHITECTURE_
#define _X86_ARCHITECTURE_

#include <sstream>

#include <medusa/types.hpp>
#include <medusa/architecture.hpp>
#include <medusa/binary_stream.hpp>
#include <medusa/instruction.hpp>
#include <medusa/information.hpp>
#include <medusa/context.hpp>
#include <medusa/medusa.hpp>

#include "x86.hpp"
#include "x86_const.hpp"

#if defined(_WIN32) || defined(WIN32)
# ifdef arch_x86_EXPORTS
#  define ARCH_X86_EXPORT __declspec(dllexport)
# else
#  define ARCH_X86_EXPORT __declspec(dllimport)
# endif
#else
# define ARCH_X86_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" ARCH_X86_EXPORT Architecture* GetArchitecture(void);

class X86Architecture : public Architecture
{
private:
  class X86CpuInformation : public CpuInformation
  {
  public:
    virtual char const* ConvertIdentifierToName(u32 Id) const;
    virtual u32 ConvertNameToIdentifier(std::string const& rName) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id) const;
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1) const;
  } m_CpuInfo;

  class X86CpuContext : public CpuContext
  {
  public:
    X86CpuContext(u8 Bits, CpuInformation const& rCpuInfo) : CpuContext(rCpuInfo), m_Bits(Bits) { memset(&m_Context, 0x0, sizeof(m_Context)); }
    virtual bool ReadRegister (u32 Register, void*       pValue, u32 Size) const;
    virtual bool WriteRegister(u32 Register, void const* pValue, u32 Size, bool SignExtend = false);
    virtual void* GetRegisterAddress(u32 Register);
    virtual void* GetContextAddress(void) { return &m_Context; }
    virtual u16 GetRegisterOffset(u32 Register);
    virtual void GetRegisters(RegisterList& RegList) const;
    virtual bool Translate(Address const& rLogicalAddress, u64& rLinearAddress) const;
    virtual std::string ToString(void) const;

  private:
    union X86Register
    {
      u64 r;
      u32 e;
      u16 w;
      // LATER: This is not portable http://stackoverflow.com/questions/1490092/c-c-force-bit-field-order-and-alignment
      struct { u16 l : 8; u16 h : 8; } x;
    };
    struct Context
    {
      X86Register a, b, c, d, si, di, bp, sp, ip, r8, r9, r10, r11, r12, r13, r14, r15;
      u16 cs, ds, es, ss, fs, gs;
      u32 flags;
    } m_Context;

    u8 m_Bits;
  };

public:
  X86Architecture(void);
  ~X86Architecture(void) {}

  virtual std::string           GetName(void) const { return "Intel x86"; }
  virtual bool                  Translate(Address const& rVirtAddr, TOffset& rPhysOff) { return false; }
  virtual EEndianness           GetEndianness(void) { return LittleEndian; }
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  virtual NamedModeVector       GetModes(void) const
  {
    NamedModeVector X86Modes;
    X86Modes.reserve(3);
    // LATER: Check if it's not better to expose real, protected(16-bit/32-bit), long
    X86Modes.push_back(NamedMode("16-bit", X86_Bit_16));
    X86Modes.push_back(NamedMode("32-bit", X86_Bit_32));
    X86Modes.push_back(NamedMode("64-bit", X86_Bit_64));
    return X86Modes;
  }

  virtual u8                    GetDefaultMode(Address const&) const
  {
    return 0;
  }

  virtual bool FormatOperand(
    Document      const& rDoc,
    Address       const& rAddress,
    Instruction   const& rInstruction,
    Operand       const& rOperand,
    u8                   OperandNo,
    PrintData          & rPrintData) const;

  virtual bool FormatInstruction(
    Document      const& rDoc,
    Address       const& rAddr,
    Instruction   const& rInsn,
    PrintData          & rPrintData) const;

  virtual CpuInformation const* GetCpuInformation(void) const { return &m_CpuInfo; }
  virtual CpuContext*           MakeCpuContext(void) const { return new X86CpuContext(0, m_CpuInfo); }
  virtual MemoryContext*        MakeMemoryContext(void) const { return new MemoryContext(m_CpuInfo); }

  virtual Expression*           UpdateFlags(Instruction& rInsn, Expression* pResultExpr);
  virtual OperationExpression*  SetFlags(Instruction& rInsn, u32 Flags);
  virtual OperationExpression*  ResetFlags(Instruction& rInsn, u32 Flags);
  virtual ConditionExpression*  TestFlags(Instruction& rInsn, u32 Flags);
  virtual ConditionExpression*  TestNotFlags(Instruction& rInsn, u32 Flags);
  virtual OperationExpression*  ExtractFlag(Instruction& rInsn, u32 Flag);

private:
#include "x86_operand.ipp"
#include "x86_opcode.ipp"

  u32 ConvertFlagIdToMask(u32 Flags)
  {
    u32 FlagsMask = 0;
#define CONVERT_FLAG_ID_TO_MASK(fl) if (Flags & X86_Fl##fl) FlagsMask |= (1 << X86_##fl##Bit)
    CONVERT_FLAG_ID_TO_MASK(Cf);
    CONVERT_FLAG_ID_TO_MASK(Pf);
    CONVERT_FLAG_ID_TO_MASK(Af);
    CONVERT_FLAG_ID_TO_MASK(Zf);
    CONVERT_FLAG_ID_TO_MASK(Sf);
    CONVERT_FLAG_ID_TO_MASK(Tf);
    CONVERT_FLAG_ID_TO_MASK(If);
    CONVERT_FLAG_ID_TO_MASK(Df);
    CONVERT_FLAG_ID_TO_MASK(Of);
#undef CONVERT_FLAG_ID_TO_MASK
    return FlagsMask;
  }

private:
  static const char * m_Mnemonic[];

  void                ApplySegmentOverridePrefix(Instruction& rInsn, Operand* pOprd);
};

#endif // !_X86_ARCHITECTURE_