#include "avr8.hpp"
#include <medusa/module.hpp>
#include <medusa/instruction.hpp>
#include <medusa/function.hpp>
#include <medusa/expression.hpp>

std::string Avr8OperatingSystem::GetName(void) const
{
  return "AVR8";
}

bool Avr8OperatingSystem::InitializeCpuContext(Document const& rDoc, CpuContext& rCpuCtxt) const
{
  return true;
}

bool Avr8OperatingSystem::InitializeMemoryContext(Document const& rDoc, MemoryContext& rMemCtxt) const
{
  return true;
}

bool Avr8OperatingSystem::IsSupported(Loader const& rLdr, Architecture const& rArch) const
{
  return true; //
}

bool Avr8OperatingSystem::ProvideDetails(Document& rDoc) const
{
  return true;
}

bool Avr8OperatingSystem::AnalyzeFunction(Document& rDoc, Address const& rAddress)
{
    return true;
}

bool Avr8OperatingSystem::GetValueDetail(Id ValueId, ValueDetail& rValDtl) const
{
  return true;
}

bool Avr8OperatingSystem::GetFunctionDetail(Id FunctionId, FunctionDetail& rFcnDtl) const
{
  return true;
}

bool Avr8OperatingSystem::GetStructureDetail(Id StructureId, StructureDetail& rStructDtl) const
{
  return true;
}
