#ifndef ARCH_X86_HPP
#define ARCH_X86_HPP

#include <sstream>

#include <medusa/types.hpp>
#include <medusa/extend.hpp>
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
    X86CpuInformation(void) : CpuInformation(MEDUSA_ARCH_TAG('x', '8', '6')) {}
    virtual char const* ConvertIdentifierToName(u32 Id) const;
    virtual u32 ConvertNameToIdentifier(std::string const& rName) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id) const;
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1) const;
    virtual bool NormalizeRegister(u32 Id, u8 Mode, u32& rExtId, u64& rMask) const;
  } m_CpuInfo;

  class X86CpuContext : public CpuContext
  {
  public:
    X86CpuContext(u8 Bits, CpuInformation const& rCpuInfo);
    virtual bool ReadRegister (u32 Reg, void*       pVal, u32 BitSize) const;
    virtual bool WriteRegister(u32 Reg, void const* pVal, u32 BitSize);
    virtual void* GetRegisterAddress(u32 Register);
    virtual void* GetContextAddress(void) { return &m_Context; }
    virtual u16 GetRegisterOffset(u32 Register);
    virtual void GetRegisters(RegisterList& RegList) const;
    virtual bool Translate(Address const& rLogicalAddress, u64& rLinearAddress) const;
    virtual u8 GetMode(void) const;
    virtual void SetMode(u8 Mode);
    virtual bool GetAddress(AddressKind AddrKind, Address& rAddr) const;
    virtual bool SetAddress(AddressKind AddrKind, Address const& rAddr);
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
      // http://en.wikipedia.org/wiki/FLAGS_register
      bool CF, PF, AF, ZF, SF, TF, IF, DF, OF, IOPL, NT, RF, VM, AC, VIF, VIP, ID;
      u128 xyzmm[32];
    } m_Context;

    u8 m_Bits;
  };

public:
  X86Architecture(void);
  ~X86Architecture(void) {}

  virtual std::string           GetName(void) const { return "x86"; }
  virtual bool                  Translate(Address const& rVirtAddr, OffsetType& rPhysOff) { return false; }
  virtual EEndianness           GetEndianness(void) { return LittleEndian; }
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
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

  virtual bool FormatInstruction(
    Document      const& rDoc,
    Address       const& rAddr,
    Instruction   const& rInsn,
    PrintData          & rPrintData) const;

  virtual CpuInformation    const* GetCpuInformation(void) const { return &m_CpuInfo; }
  virtual CallingConvention const* GetCallingConvention(std::string const& rCallConvName, u8 Mode) const;
  virtual std::vector<std::string> GetCallingConventionNames(void) const;

  virtual CpuContext*    MakeCpuContext(void)    const { return new X86CpuContext(0, m_CpuInfo); }
  virtual MemoryContext* MakeMemoryContext(void) const { return new MemoryContext(m_CpuInfo); }

  virtual bool HandleExpression(Expression::LSPType & rExprs, std::string const& rName, Instruction& rInsn, Expression::SPType spResExpr);
  virtual bool EmitSetExecutionAddress(Expression::VSPType& rExprs, Address const& rAddr, u8 Mode);

private:
#include "x86_operand.ipp"
#include "x86_opcode.ipp"

  Expression::SPType __Decode_Ap(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_By(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Cy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Dy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Edb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Eq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Ev(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Ew(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Ey(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Eyb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Eyw(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Gb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Gw(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Gd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Gq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Gv(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Gy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Ho(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Hx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Hy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Ibs(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Jb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Jz(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Lx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_M(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Ma(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Md(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Mo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Mod(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Moq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Mp(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Mq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Mqo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Mv(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Mw(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Mx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_My(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Nq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Ob(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Ov(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Pq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Qd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Qq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Rv(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Ry(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Sw(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Ty(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Uo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Uod(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Uoq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Uq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Ux(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Vo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Vod(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Voq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Vx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Vy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_Wd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Wo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Wob(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Wod(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Woo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Woq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Woqo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Wow(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Wq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Wx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_Wy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_m16int(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_m32fp(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_m32int(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_m64fp(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_m64int(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_m80bcd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_m80dec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_m80fp(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

  Expression::SPType __Decode_b(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_w(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType __Decode_x(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);

private:
  static const char * m_Mnemonic[];
};

#endif // !ARCH_X86_HPP