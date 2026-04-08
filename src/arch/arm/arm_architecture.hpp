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
    char const* ConvertIdentifierToName(u32 Id) const override;
    u32 ConvertNameToIdentifier(std::string const& rName) const override;
    u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const override;
    u32 GetSizeOfRegisterInBit(u32 Id) const override;
    bool IsRegisterAliased(u32 Id0, u32 Id1) const override { return false; }
  } m_CpuInfo;

  class ARMCpuContext : public CpuContext
  {
  public:
    ARMCpuContext(CpuInformation const& rCpuInfo) : CpuContext(rCpuInfo) { memset(&m_Context, 0x0, sizeof(m_Context)); }
    bool ReadRegister (u32 Reg, void*       pVal, u32 BitSize) const override;
    bool WriteRegister(u32 Reg, void const* pVal, u32 BitSize) override;
    bool Translate(Address const& rLogicalAddress, u64& rLinearAddress) const override;
    u8 GetMode(void) const override;
    void SetMode(u8 Mode) override;
    bool GetAddress(AddressKind AddrKind, Address& rAddr) const override;
    bool SetAddress(AddressKind AddrKind, Address const& rAddr) override;
    std::string ToString(void) const override;

    void* GetRegisterAddress(u32 Register) override;
    void* GetContextAddress(void) override;
    u16   GetRegisterOffset(u32 Register) override;

    void  GetRegisters(RegisterList& RegList) const override;

  private:
    struct
    {
      u32 Registers[16];
      u32 CPSR; //http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0210c/CIHIAAGE.html
    } m_Context;
  };

public:
  ArmArchitecture(void);
  ~ArmArchitecture(void) override {}

  std::string           GetName(void) const override                                   { return "ARM"; }
  bool                  Translate(Address const& rVirtAddr, OffsetType& rPhysOff) override { return false; }
  Address               CurrentAddress(Address const& rAddr, Instruction const& rInsn) const override;
  EEndianness           GetEndianness(void) override                                   { return LittleEndian; }
  bool                  Disassemble(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode) override;
  NamedModeVector       GetModes(void) const override
  {
    NamedModeVector ArmModes;
    ArmModes.reserve(4);
    ArmModes.push_back(NamedMode("arm",     ARM_ModeArm));
    ArmModes.push_back(NamedMode("thumb",   ARM_ModeThumb));
    return ArmModes;
  }
  u8                    GetDefaultMode(Address const& rAddress) const override
  { return rAddress.GetOffset() & 1 ? ARM_ModeThumb : ARM_ModeArm; }

  bool FormatOperand(
    Document      const& rDoc,
    Address       const& rAddress,
    Instruction   const& rInstruction,
    u8                   OperandNo,
    PrintData          & rPrintData) const override;

  bool                  FormatInstruction(
    Document      const& rDoc,
    Address       const& rAddr,
    Instruction   const& rInsn,
    PrintData          & rPrintData) const override;

  CpuInformation    const* GetCpuInformation(void) const override                                        { static ARMCpuInformation ArmCpuInfo; return &ArmCpuInfo; }
  CallingConvention const* GetCallingConvention(std::string const& rCallConvName, u8 Mode) const override;
  std::vector<std::string> GetCallingConventionNames(void) const override                                { return{ "aapcs" }; }

  CpuContext*           MakeCpuContext(void) const override                            { return new ARMCpuContext(*GetCpuInformation()); }
  MemoryContext*        MakeMemoryContext(void) const override                         { return new MemoryContext(*GetCpuInformation()); }

  bool                  HandleExpression(Expression::LSPType & rExprs, std::string const& rName, Instruction& rInsn, Expression::SPType spResExpr) override;
  bool                  EmitSetExecutionAddress(Expression::VSPType& rExprs, Address const& rAddr, u8 Mode) override;

#include "arm_opcode.ipp"
};

#endif // !ARM_ARCHITECTURE_HPP
