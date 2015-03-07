#include <Windows.h>

#include <boost/filesystem.hpp>

#include "medusa/namespace.hpp"
#include "medusa/module.hpp"

MEDUSA_NAMESPACE_BEGIN

char const* Module::GetExtension(void)
{
  return "dll";
}

void* Module::ImplLoadLibrary(boost::filesystem::path const& ModulePath)
{
  boost::filesystem::path Path = ModulePath;

  auto ModuleName = Path.stem().string();
  void* pModule = m_ModuleMap[ModuleName];

  if (pModule == nullptr)
  {
    pModule = LoadLibraryW(ModulePath.c_str());
    m_ModuleMap[ModuleName] = pModule;
  }

  return pModule;
}

#ifdef __MINGW32__
FARPROC WINAPI Module::ImplGetFunctionAddress(void* pModule, std::string const& FunctionName)
{
  return GetProcAddress(static_cast<HMODULE>(pModule), FunctionName.c_str());
}
#else
void* Module::ImplGetFunctionAddress(void* pModule, std::string const& FunctionName)
{
  return GetProcAddress(static_cast<HMODULE>(pModule), FunctionName.c_str());
}
#endif

MEDUSA_NAMESPACE_END