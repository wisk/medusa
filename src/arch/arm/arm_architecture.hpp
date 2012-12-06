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
public:
  ArmArchitecture(void) : Architecture(MEDUSA_ARCH_TAG('a', 'r', 'm')) {}
  ~ArmArchitecture(void) {}

  virtual std::string           GetName(void)                                          { return "ARM"; }
  virtual bool                  Translate(Address const& rVirtAddr, TOffset& rPhysOff) { return false; }
  virtual EEndianness           GetEndianness(void)                                    { return LittleEndian; }
  virtual bool                  Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
  virtual void                  FormatInstruction(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddress, Instruction& rInsn) const;
  virtual void                  FillConfigurationModel(ConfigurationModel& rCfgMdl);
  virtual CpuInformation const* GetCpuInformation(void) const                { return nullptr; }

private:
  std::string RegisterToString(u32 Register) const;

#include "arm_opcode.ipp"
};

#endif // !__ARM_ARCHITECTURE_HPP__