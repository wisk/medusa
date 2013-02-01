#include "windows.hpp"

WindowsOperatingSystem::~WindowsOperatingSystem(void)
{
}

std::string WindowsOperatingSystem::GetName(void) const
{
  return "MS Windows";
}

bool WindowsOperatingSystem::InitializeCpuContext(CpuContext& rCpuCtxt) const
{
  // TODO: set fs and/or gs to _TEB
  return false;
}

bool WindowsOperatingSystem::InitializeMemoryContext(MemoryContext& rMemCtxt) const
{
  // TODO: create a fake _TEB/_PEB
  return false;
}

bool WindowsOperatingSystem::IsSupported(Loader const& rLdr, Architecture const& rArch) const
{
  // At this time, only Windows on x86 is supported
  if ((rLdr.GetName() == "PE" || rLdr.GetName() == "PE+") && rArch.GetName() == "Intel x86")
    return true;

  return false;
}