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

  class X86CpuContext : public CpuContext
  {
  public:
    X86CpuContext(Configuration const& rCfg, CpuInformation const& rCpuInfo) : CpuContext(rCpuInfo), m_rCfg(rCfg) { memset(&m_Context, 0x0, sizeof(m_Context)); }
    virtual bool ReadRegister (u32 Register, void*       pValue, u32 Size) const;
    virtual bool WriteRegister(u32 Register, void const* pValue, u32 Size);
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
    struct
    {
      X86Register a, b, c, d, si, di, bp, sp, ip, r8, r9, r10, r11, r12, r13, r14, r15;
      u16 cs, ds, es, ss, fs, gs;
      u32 flags;
    } m_Context;

    Configuration const& m_rCfg;
  };

public:
  X86Architecture(void)
    : Architecture(MEDUSA_ARCH_TAG('x','8','6'))
    , m_CpuInfo(m_Cfg)
    , m_Mode(0x0)
    , m_CpuModel(X86_Arch_Sse4a)
    , m_ProcType(X86_ProcType_INTEL)
  {}
  ~X86Architecture(void) {}

  virtual std::string           GetName(void) const { return "Intel x86"; }
  virtual bool                  Translate(Address const& rVirtAddr, TOffset& rPhysOff) { return false; }
  virtual EEndianness           GetEndianness(void) { return LittleEndian; }
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  virtual void                  FormatInstruction(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr, Instruction& rInsn) const;
  virtual void                  FillConfigurationModel(ConfigurationModel& rCfgMdl);
  virtual CpuInformation const* GetCpuInformation(void) const { return &m_CpuInfo; }
  virtual CpuContext*           MakeCpuContext(void) const { return new X86CpuContext(m_Cfg, m_CpuInfo); }
  virtual MemoryContext*        MakeMemoryContext(void) const { return new MemoryContext(m_CpuInfo); }

private:
#include "x86_operand.ipp"
#include "x86_opcode.ipp"
#include "x86_flag.ipp"

private:
  static const char * m_Mnemonic[];

  void                FormatOperand(std::ostringstream& rInsnBuf, Database const& rDb, TOffset Offset, Instruction& rInsn, Operand* pOprd) const;
  void                ApplySegmentOverridePrefix(Instruction& rInsn, Operand* pOprd);

  u32 m_Mode;     /* Unused */
  u32 m_CpuModel; /* Unused */
  u32 m_ProcType; /* Unused */
};

#endif // !_X86_ARCHITECTURE_