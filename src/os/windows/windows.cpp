#include "windows.hpp"
#include "stack_analyzer.hpp"
#include "medusa/module.hpp"

std::string WindowsOperatingSystem::GetName(void) const
{
  return "MS Windows";
}

bool WindowsOperatingSystem::InitializeCpuContext(Document const& rDoc, CpuContext& rCpuCtxt) const
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

  auto StartAddr = rDoc.GetAddressFromLabelName("start");
  u64 StartAddrVal = StartAddr.GetOffset();
  auto IdD = rCpuInfo.ConvertNameToIdentifier("rdx"); // it doesn't matter to use rdx instead of ecx or cx
  if (rCpuCtxt.WriteRegister(IdD, &StartAddrVal, sizeof(StartAddrVal)) == false)
    return false;

  return true;
}

bool WindowsOperatingSystem::InitializeMemoryContext(Document const& rDoc, MemoryContext& rMemCtxt) const
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

bool WindowsOperatingSystem::ProvideDetails(Document& rDoc) const
{
  // TODO: determine if the PE is 32-bit or 64-bit

 u16 Bitness = 32;

  TypeDetail VoidType("VOID", TypeDetail::VoidType, 0);
  TypedValueDetail::List ExitProcessParams;
  ExitProcessParams.push_back(TypedValueDetail(
      "UINT", TypeDetail::IntegerType, 32,
      "uExitCode", Id(), ValueDetail::DecimalType));
  FunctionDetail ExitProcessFunc("kernel32.dll!ExitProcess", VoidType, ExitProcessParams);
  rDoc.SetFunctionDetail(Sha1("kernel32.dll!ExitProcess"), ExitProcessFunc);

  TypeDetail IntType("int", TypeDetail::IntegerType, 32);
  TypedValueDetail::List MessageBoxAParams;
  MessageBoxAParams.push_back(TypedValueDetail(
        "HWND", TypeDetail::TypedefType, Bitness,
        "hWnd", Id(), ValueDetail::HexadecimalType));
  MessageBoxAParams.push_back(TypedValueDetail(
        "LPCSTR", TypeDetail::TypedefType, Bitness,
        "lpText", Id(), ValueDetail::ReferenceType));
  MessageBoxAParams.push_back(TypedValueDetail(
        "LPCSTR", TypeDetail::TypedefType, Bitness,
        "lpCaption", Id(), ValueDetail::ReferenceType));
  MessageBoxAParams.push_back(TypedValueDetail(
        "UINT", TypeDetail::IntegerType, 32,
        "uType", Id(), ValueDetail::ConstantType, Id()));
  FunctionDetail MessageBoxAFunc("user32.dll!MessageBoxA", IntType, MessageBoxAParams);
  rDoc.SetFunctionDetail(Sha1("user32.dll!MessageBoxA"), MessageBoxAFunc);

  return true;
}
