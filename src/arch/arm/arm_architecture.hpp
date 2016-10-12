#ifndef ARM_ARCHITECTURE_HPP
#define ARM_ARCHITECTURE_HPP

#include "arm_def.hpp"

#include <sstream>

#include <medusa/types.hpp>
#include <medusa/architecture.hpp>
#include <medusa/binary_stream.hpp>
#include <medusa/instruction.hpp>
#include <medusa/medusa.hpp>
#include <medusa/bits.hpp>
#include <medusa/extend.hpp>

#if defined(_WIN32) || defined(WIN32)
# ifdef arch_arm_EXPORTS
#  define ARCH_ARM_EXPORT __declspec(dllexport)
# else
#  define ARCH_ARM_EXPORT __declspec(dllimport)
# endif
#else
# define ARCH_ARM_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" ARCH_ARM_EXPORT Architecture* GetArchitecture(void);

class ArmArchitecture : public Architecture
{
  class ARMCpuInformation : public CpuInformation
  {
  public:
    ARMCpuInformation(void) : CpuInformation(MEDUSA_ARCH_TAG('a', 'r', 'm')) {}
    virtual char const* ConvertIdentifierToName(u32 Id) const;
    virtual u32 ConvertNameToIdentifier(std::string const& rName) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id) const;
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1) const { return false; }
  } m_CpuInfo;

  class ARMCpuContext : public CpuContext
  {
  public:
    ARMCpuContext(CpuInformation const& rCpuInfo) : CpuContext(rCpuInfo) { memset(&m_Context, 0x0, sizeof(m_Context)); }
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
    struct
    {
      u32 Registers[16];
      u32 CPSR; //http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0210c/CIHIAAGE.html
    } m_Context;
  };

public:
  ArmArchitecture(void);
  ~ArmArchitecture(void) {}

  virtual std::string           GetName(void) const                                    { return "ARM"; }
  virtual bool                  Translate(Address const& rVirtAddr, OffsetType& rPhysOff) { return false; }
  virtual Address               CurrentAddress(Address const& rAddr, Instruction const& rInsn) const;
  virtual EEndianness           GetEndianness(void)                                    { return LittleEndian; }
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode);
  virtual NamedModeVector       GetModes(void) const
  {
    NamedModeVector ArmModes;
    ArmModes.reserve(4);
    ArmModes.push_back(NamedMode("arm",     ARM_ModeArm));
    ArmModes.push_back(NamedMode("thumb",   ARM_ModeThumb));
    return ArmModes;
  }
  virtual u8                    GetDefaultMode(Address const& rAddress) const
  { return rAddress.GetOffset() & 1 ? ARM_ModeThumb : ARM_ModeArm; }

  virtual bool FormatOperand(
    Document      const& rDoc,
    Address       const& rAddress,
    Instruction   const& rInstruction,
    u8                   OperandNo,
    PrintData          & rPrintData) const;

  virtual bool                  FormatInstruction(
    Document      const& rDoc,
    Address       const& rAddr,
    Instruction   const& rInsn,
    PrintData          & rPrintData) const;

  virtual CpuInformation    const* GetCpuInformation(void) const                                         { static ARMCpuInformation ArmCpuInfo; return &ArmCpuInfo; }
  virtual CallingConvention const* GetCallingConvention(std::string const& rCallConvName, u8 Mode) const;
  virtual std::vector<std::string> GetCallingConventionNames(void) const                                 { return{ "aapcs" }; }

  virtual CpuContext*           MakeCpuContext(void) const                             { return new ARMCpuContext(*GetCpuInformation()); }
  virtual MemoryContext*        MakeMemoryContext(void) const                          { return new MemoryContext(*GetCpuInformation()); }

  virtual bool                  HandleExpression(Expression::LSPType & rExprs, std::string const& rName, Instruction& rInsn, Expression::SPType spResExpr);
  virtual bool                  EmitSetExecutionAddress(Expression::VSPType& rExprs, Address const& rAddr, u8 Mode);

#include "arm_opcode.ipp"
};

#endif // !ARM_ARCHITECTURE_HPP
