#ifndef _X86_ARCHITECTURE_
#define _X86_ARCHITECTURE_

#include <sstream>

#include <medusa/types.hpp>
#include <medusa/architecture.hpp>
#include <medusa/binary_stream.hpp>
#include <medusa/instruction.hpp>
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
    X86CpuInformation(Configuration const& rCfg) : m_rCfg(rCfg) {}
    virtual char const* ConvertIdentifierToName(u32 Id) const;
    virtual u32 GetRegisterByType(CpuInformation::Type RegType) const;
    virtual u32 GetSizeOfRegisterInBit(u32 Id) const;

  private:
    Configuration const& m_rCfg;
  } m_CpuInfo;

public:
  X86Architecture(void)
    : Architecture(MEDUSA_ARCH_TAG('x','8','6'))
    , m_CpuInfo(m_Cfg)
    , m_Mode(0x0)
    , m_CpuModel(X86_Arch_Sse4a)
    , m_ProcType(X86_ProcType_INTEL)
  {}
  ~X86Architecture(void) {}

  virtual std::string GetName(void) { return "Intel x86"; }
  virtual bool        Translate(Address const& rVirtAddr, TOffset& rPhysOff) { return false; }
  virtual EEndianness GetEndianness(void) { return LittleEndian; }
  virtual bool        Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  virtual void        FormatInstruction(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr, Instruction& rInsn) const;
  virtual void        FillConfigurationModel(ConfigurationModel& rCfgMdl);
  virtual CpuInformation const* GetCpuInformation(void) const { return &m_CpuInfo; }

private:
#include "x86_operand.ipp"
#include "x86_opcode.ipp"

private:
  static const char * m_Mnemonic[];

  void                FormatOperand(std::ostringstream& rInsnBuf, Database const& rDb, TOffset Offset, Instruction& rInsn, Operand* pOprd) const;
  void                ApplySegmentOverridePrefix(Instruction &rInsn);

  u32 m_Mode;     /* Unused */
  u32 m_CpuModel; /* Unused */
  u32 m_ProcType; /* Unused */
};

#endif // !_X86_ARCHITECTURE_