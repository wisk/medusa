#ifndef ARCH_LR35902_HPP
#define ARCH_LR35902_HPP

#include "medusa/types.hpp"
#include "medusa/extend.hpp"
#include "medusa/architecture.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/instruction.hpp"
#include "medusa/context.hpp"

#if defined(_WIN32) || defined(WIN32)
# ifdef arch_lr35902_EXPORTS
#  define ARCH_LR35902_EXPORT __declspec(dllexport)
# else
#  define ARCH_LR35902_EXPORT __declspec(dllimport)
# endif
#else
# define ARCH_LR35902_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" ARCH_LR35902_EXPORT Architecture* GetArchitecture();

class LR35902Architecture : public Architecture
{
private:
  class LR35902CpuInformation : public CpuInformation
  {
  public:
    LR35902CpuInformation() : CpuInformation(MEDUSA_ARCH_TAG('l', 'r', '3')) {}
    char const* ConvertIdentifierToName(u32 Id) const override;
    u32 ConvertNameToIdentifier(std::string const& rName) const override;
    u32 GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const override;
    u32 GetSizeOfRegisterInBit(u32 Id) const override;
    bool IsRegisterAliased(u32 Id0, u32 Id1) const override;
  } m_CpuInfo;

public:
  enum class LR35902Mode : u8
  {
    Default = 1,
  };

  enum class LR35902InsnPrefix : u8
  {
    None,
    NotFlag,
  };

  LR35902Architecture() : Architecture(MEDUSA_ARCH_TAG('l','r','3')) {}

  std::string           GetName() const override { return "Sharp LR35902"; }
  bool                  Translate(Address const& rVirtAddr, OffsetType& rPhyslOff) override;
  bool                  Disassemble(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode) override;
  NamedModeVector       GetModes() const override;
  EEndianness           GetEndianness() override { return LittleEndian; }
  CpuInformation const* GetCpuInformation() const override { return &m_CpuInfo; }
  CpuContext*           MakeCpuContext() const override { return nullptr; }
  MemoryContext*        MakeMemoryContext() const override { return new MemoryContext(m_CpuInfo); }

  bool FormatInstruction(
    Document      const& rDoc,
    Address       const& rAddr,
    Instruction   const& rInsn,
    PrintData          & rPrintData) const override;
};

enum class LR35902Identifiers : u32
{
  Reg_Unknown,

  Flg_C,
  Flg_H,
  Flg_N,
  Flg_Z,

  Reg_A,
  Reg_B,
  Reg_C,
  Reg_D,
  Reg_E,
  Reg_F,
  Reg_H,
  Reg_L,
  Reg_AF,
  Reg_BC,
  Reg_DE,
  Reg_HL,
  Reg_Sp,
  Reg_Pc,
};

// Convenience aliases for backward compatibility with generated code
inline constexpr u32 LR35902_Reg_Unknown = static_cast<u32>(LR35902Identifiers::Reg_Unknown);
inline constexpr u32 LR35902_Flg_C       = static_cast<u32>(LR35902Identifiers::Flg_C);
inline constexpr u32 LR35902_Flg_H       = static_cast<u32>(LR35902Identifiers::Flg_H);
inline constexpr u32 LR35902_Flg_N       = static_cast<u32>(LR35902Identifiers::Flg_N);
inline constexpr u32 LR35902_Flg_Z       = static_cast<u32>(LR35902Identifiers::Flg_Z);
inline constexpr u32 LR35902_Reg_A       = static_cast<u32>(LR35902Identifiers::Reg_A);
inline constexpr u32 LR35902_Reg_B       = static_cast<u32>(LR35902Identifiers::Reg_B);
inline constexpr u32 LR35902_Reg_C       = static_cast<u32>(LR35902Identifiers::Reg_C);
inline constexpr u32 LR35902_Reg_D       = static_cast<u32>(LR35902Identifiers::Reg_D);
inline constexpr u32 LR35902_Reg_E       = static_cast<u32>(LR35902Identifiers::Reg_E);
inline constexpr u32 LR35902_Reg_F       = static_cast<u32>(LR35902Identifiers::Reg_F);
inline constexpr u32 LR35902_Reg_H       = static_cast<u32>(LR35902Identifiers::Reg_H);
inline constexpr u32 LR35902_Reg_L       = static_cast<u32>(LR35902Identifiers::Reg_L);
inline constexpr u32 LR35902_Reg_AF      = static_cast<u32>(LR35902Identifiers::Reg_AF);
inline constexpr u32 LR35902_Reg_BC      = static_cast<u32>(LR35902Identifiers::Reg_BC);
inline constexpr u32 LR35902_Reg_DE      = static_cast<u32>(LR35902Identifiers::Reg_DE);
inline constexpr u32 LR35902_Reg_HL      = static_cast<u32>(LR35902Identifiers::Reg_HL);
inline constexpr u32 LR35902_Reg_Sp      = static_cast<u32>(LR35902Identifiers::Reg_Sp);
inline constexpr u32 LR35902_Reg_Pc      = static_cast<u32>(LR35902Identifiers::Reg_Pc);
inline constexpr u8  LR35902_Insn_Prefix_None    = static_cast<u8>(LR35902Architecture::LR35902InsnPrefix::None);
inline constexpr u8  LR35902_Insn_Prefix_NotFlag = static_cast<u8>(LR35902Architecture::LR35902InsnPrefix::NotFlag);

#endif // !ARCH_LR35902_HPP
