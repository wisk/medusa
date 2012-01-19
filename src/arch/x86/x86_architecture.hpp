#ifndef _X86_ARCHITECTURE_
#define _X86_ARCHITECTURE_

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
public:
  X86Architecture(void) : m_Mode(0x0), m_CpuModel(X86_Arch_Sse4a), m_ProcType(X86_ProcType_INTEL) {}
  ~X86Architecture(void) {}

  virtual std::string GetName(void) { return "Intel x86"; }
  virtual bool        Translate(Address const& rVirtAddr, TOffset& rPhysOff) { return false; }
  virtual std::string FormatOperand(TOffset Offset, Instruction const& rInsn, Operand const* pOprd);
  virtual void        FormatInstruction(Database const& rDatabase, Address const& rAddr, Instruction& rInsn);
  virtual EEndianness GetEndianness(void) { return LittleEndian; }
  virtual bool        Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
  {
    u8 Opcode;
    rBinStrm.Read(Offset, Opcode);
    bool Res = (this->*m_Table1[Opcode])(rBinStrm, Offset + 1, rInsn);
    ApplySegmentOverridePrefix(rInsn);
    for (u32 i = 0; i < OPERAND_NO; ++i) /* TMP */
      rInsn.Operand(i)->SetName(FormatOperand(Offset, rInsn, rInsn.Operand(i)).c_str());
    return Res;
  }
  virtual void FillConfigurationModel(ConfigurationModel& rCfgMdl);

  void ApplySegmentOverridePrefix(Instruction &rInsn);

private:
#include "x86_operand.ipp"
#include "x86_opcode.ipp"

private:
  static const char *m_Mnemonic[];

  u32 m_Mode;     /* DUMMY */
  u32 m_CpuModel; /* DUMMY */
  u32 m_ProcType; /* DUMMY */
};

#endif // !_X86_ARCHITECTURE_

