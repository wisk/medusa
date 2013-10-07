#include <dlfcn.h>
#include <boost/filesystem.hpp>

#include "medusa/namespace.hpp"
#include "medusa/module.hpp"
#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

wchar_t const* Module::GetExtension(void)
{
return L"so";
}

void* Module::ImplLoadLibrary(std::wstring const& rModulePath)
{
  boost::filesystem::path Path = rModulePath;

  std::wstring ModuleName = Path.stem().wstring();
  void* pModule = m_ModuleMap[ModuleName];

  char* pUtf8ModulePath = new char[rModulePath.length() + 1];
  if (wcstombs(pUtf8ModulePath, rModulePath.c_str(), rModulePath.length() + 1) == -1)
    throw Exception_System(L"wcstombs");

  if (pModule == nullptr)
  {
    pModule = dlopen(pUtf8ModulePath, RTLD_LAZY);
    m_ModuleMap[ModuleName] = pModule;
  }

  delete[] pUtf8ModulePath;

  if (pModule == nullptr)
    Log::Write("core") << dlerror() << LogEnd;

  return pModule;
}

void* Module::ImplGetFunctionAddress(void* pModule, std::string const& FunctionName)
{
  return dlsym(pModule, FunctionName.c_str());
}

MEDUSA_NAMESPACE_END
