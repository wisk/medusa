#include <dlfcn.h>
#include <boost/filesystem.hpp>

#include "medusa/namespace.hpp"
#include "medusa/module.hpp"
#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

char const* Module::GetExtension(void)
{
return "so";
}

void* Module::ImplLoadLibrary(boost::filesystem::path const& rModulePath)
{
  auto ModuleName = rModulePath.stem().string();
  void* pModule = m_ModuleMap[ModuleName];

  if (pModule == nullptr)
  {
    pModule = dlopen(rModulePath.string().c_str(), RTLD_LAZY);
    m_ModuleMap[ModuleName] = pModule;
  }

  if (pModule == nullptr)
    Log::Write("core") << dlerror() << LogEnd;

  return pModule;
}

void* Module::ImplGetFunctionAddress(void* pModule, std::string const& FunctionName)
{
  return dlsym(pModule, FunctionName.c_str());
}

MEDUSA_NAMESPACE_END
