#ifndef ARCH_X86_CALLING_CONVENTION_HPP
#define ARCH_X86_CALLING_CONVENTION_HPP

#include "medusa/namespace.hpp"
#include "x86_const.hpp"
#include "medusa/calling_convention.hpp"

MEDUSA_NAMESPACE_USE

class CdeclCallingConvention : public CallingConvention
{
public:
  CdeclCallingConvention(u8 Mode, CpuInformation const& rCpuInfo) : m_Mode(Mode), m_rCpuInfo(rCpuInfo) {}

  virtual bool               GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt,
    u16 ParamNr, IntType& rParamVal)                                                          const;
  virtual bool               ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo)                                                                              const;
  virtual bool               ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo, IntType const& rRetVal)                                                      const;

  virtual Expression::SPType EmitGetParameter(u16 ParamNr, ValueType ParamTy)                 const;
  virtual Expression::SPType EmitReturnFromFunction(u16 ParamNo)                              const;
  virtual Expression::SPType EmitReturnValueFromFunction(u16 ParamNo, IntType const& rRetVal) const;

  virtual bool               AnalyzeArgument(
    Expression::SPType spExpr, u16& rArgNr, ValueType& rArgTy)                                const;
  virtual bool               AnalyzeParameter(
    Expression::SPType spExpr, u16& rParamNr, ValueType& rParamTy)                            const;
  virtual RegisterType       GetRegisterType(u32 Id)                                          const;
  virtual StackCleanerType   StackCleanupBy(void)                                             const;

private:
  u8 m_Mode;
  CpuInformation const& m_rCpuInfo;
};

class StdCallCallingConvention : public CallingConvention
{
public:
  StdCallCallingConvention(u8 Mode, CpuInformation const& rCpuInfo) : m_Mode(Mode), m_rCpuInfo(rCpuInfo) {}

  virtual bool               GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt,
    u16 ParamNr, IntType& rParamVal)                                                          const;
  virtual bool               ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo)                                                                              const;
  virtual bool               ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo, IntType const& rRetVal)                                                      const;

  virtual Expression::SPType EmitGetParameter(u16 ParamNr, ValueType ParamTy)                 const;
  virtual Expression::SPType EmitReturnFromFunction(u16 ParamNo)                              const;
  virtual Expression::SPType EmitReturnValueFromFunction(u16 ParamNo, IntType const& rRetVal) const;

  virtual bool               AnalyzeArgument(
    Expression::SPType spExpr, u16& rArgNr, ValueType& rArgTy)                                const;
  virtual bool               AnalyzeParameter(
    Expression::SPType spExpr, u16& rParamNr, ValueType& rParamTy)                            const;
  virtual RegisterType       GetRegisterType(u32 Id)                                          const;
  virtual StackCleanerType   StackCleanupBy(void)                                             const;

private:
  u8 m_Mode;
  CpuInformation const& m_rCpuInfo;
};

class MsX64CallingConvention : public CallingConvention
{
public:
  MsX64CallingConvention(CpuInformation const& rCpuInfo) : m_rCpuInfo(rCpuInfo) {}

  virtual bool               GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt,
    u16 ParamNr, IntType& rParamVal)                                                          const;
  virtual bool               ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo)                                                                              const;
  virtual bool               ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo, IntType const& rRetVal)                                                      const;

  virtual Expression::SPType EmitGetParameter(u16 ParamNr, ValueType ParamTy)                 const;
  virtual Expression::SPType EmitReturnFromFunction(u16 ParamNo)                              const;
  virtual Expression::SPType EmitReturnValueFromFunction(u16 ParamNo, IntType const& rRetVal) const;

  virtual bool               AnalyzeArgument(
    Expression::SPType spExpr, u16& rArgNr, ValueType& rArgTy)                                const;
  virtual bool               AnalyzeParameter(
    Expression::SPType spExpr, u16& rParamNr, ValueType& rParamTy)                            const;
  virtual RegisterType       GetRegisterType(u32 Id)                                          const;
  virtual StackCleanerType   StackCleanupBy(void)                                             const;

private:
  CpuInformation const& m_rCpuInfo;
};

class SystemVCallingConvention : public CallingConvention
{
public:
  SystemVCallingConvention(CpuInformation const& rCpuInfo) : m_rCpuInfo(rCpuInfo) {}

  virtual bool               GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt,
    u16 ParamNr, IntType& rParamVal)                                                          const;
  virtual bool               ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo)                                                                              const;
  virtual bool               ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo, IntType const& rRetVal)                                                      const;

  virtual Expression::SPType EmitGetParameter(u16 ParamNr, ValueType ParamTy)                 const;
  virtual Expression::SPType EmitReturnFromFunction(u16 ParamNo)                              const;
  virtual Expression::SPType EmitReturnValueFromFunction(u16 ParamNo, IntType const& rRetVal) const;

  virtual bool               AnalyzeArgument(
    Expression::SPType spExpr, u16& rArgNr, ValueType& rArgTy)                                const;
  virtual bool               AnalyzeParameter(
    Expression::SPType spExpr, u16& rParamNr, ValueType& rParamTy)                            const;
  virtual RegisterType       GetRegisterType(u32 Id)                                          const;
  virtual StackCleanerType   StackCleanupBy(void)                                             const;

private:
  CpuInformation const& m_rCpuInfo;
};

#endif // !ARCH_X86_CALLING_CONVENTION_HPP