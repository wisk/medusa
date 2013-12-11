#ifndef __ARM_ARCHITECTURE_HPP__
#define __ARM_ARCHITECTURE_HPP__

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
    ARMCpuInformation(Configuration const& rCfg) : m_rCfg(rCfg) {}
    virtual char const* ConvertIdentifierToName(u32 Id)           const;
    virtual u32 ConvertNameToIdentifier(std::string const& rName) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType)   const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id)                    const { return 32; }
    virtual bool IsRegisterAliased(u32 Id0, u32 Id1)              const { return false; }

  private:
    Configuration const& m_rCfg;
  } m_CpuInfo;

  class ARMCpuContext : public CpuContext
  {
  public:
    ARMCpuContext(Configuration const& rCfg, CpuInformation const& rCpuInfo) : CpuContext(rCpuInfo), m_rCfg(rCfg) { memset(&m_Context, 0x0, sizeof(m_Context)); }
    virtual bool ReadRegister (u32 Register, void*       pValue, u32 Size) const;
    virtual bool WriteRegister(u32 Register, void const* pValue, u32 Size, bool SignExtend = false);
    virtual bool Translate(Address const& rLogicalAddress, u64& rLinearAddress) const;
    virtual std::string ToString(void) const;

  private:
    struct
    {
      u32 Registers[16];
      u8  Flags;
    } m_Context;
    Configuration const& m_rCfg;
  };

public:
  ArmArchitecture(void)
    : Architecture(MEDUSA_ARCH_TAG('a', 'r', 'm'))
    , m_CpuInfo(m_Cfg)
  {}
  ~ArmArchitecture(void) {}

  virtual std::string           GetName(void) const                                    { return "ARM"; }
  virtual bool                  Translate(Address const& rVirtAddr, TOffset& rPhysOff) { return false; }
  virtual EEndianness           GetEndianness(void)                                    { return LittleEndian; }
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  virtual bool                  FormatInstruction(
    Document      const& rDoc,
    BinaryStream  const& rBinStrm,
    Address       const& rAddr,
    Instruction   const& rInsn,
    std::string        & rStrCell,
    Cell::Mark::List   & rMarks) const;
  virtual void                  FillConfigurationModel(ConfigurationModel& rCfgMdl);
  virtual CpuInformation const* GetCpuInformation(void) const                          { static ARMCpuInformation ArmCpuInfo(m_Cfg); return &ArmCpuInfo; }
  virtual CpuContext*           MakeCpuContext(void) const                             { return nullptr; }
  virtual MemoryContext*        MakeMemoryContext(void) const                          { return nullptr; }

private:
  std::string RegisterToString(u32 Register) const;

#include "arm_opcode.ipp"
};

#endif // !__ARM_ARCHITECTURE_HPP__
