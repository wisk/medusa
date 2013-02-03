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
  CpuInformation const& rCpuInfo = rCpuCtxt.GetCpuInformation();
  auto IdFs = rCpuInfo.ConvertNameToIdentifier("fs");
  if (IdFs == 0)
    return false;
  u16 Fs = 0x2b;
  if (rCpuCtxt.WriteRegister(IdFs, &Fs, sizeof(Fs)) == false)
    return false;
  if (rCpuCtxt.AddMapping(Address(Fs, 0x0), 0x7fdf0000) == false)
    return false;
  return true;
}

bool WindowsOperatingSystem::InitializeMemoryContext(MemoryContext& rMemCtxt) const
{
  // TODO: create a fake _TEB/_PEB
  if (rMemCtxt.AllocateMemory(0x7fdf0000, 0x1000, nullptr) == false)
    return false;
  return true;
}

bool WindowsOperatingSystem::IsSupported(Loader const& rLdr, Architecture const& rArch) const
{
  // At this time, only Windows on x86 is supported
  if ((rLdr.GetName() == "PE" || rLdr.GetName() == "PE+") && rArch.GetName() == "Intel x86")
    return true;

  return false;
}