#ifndef _MEDUSA_MODULE_
#define _MEDUSA_MODULE_

#include "medusa/exception.hpp"
#include <map>

MEDUSA_NAMESPACE_BEGIN

//! Module is only used to load medusa modules.
class Module
{
public:
  Module(void) {}
  ~Module(void) {}

  void* Load(std::wstring const& ModulePath)
  {
    return ImplLoadLibrary(ModulePath);
  }

  static wchar_t const* GetExtension(void);

  template<typename FuncType>
  FuncType Load(std::wstring const& ModulePath, std::string const& FunctionName)
  {
    void* pModule = ImplLoadLibrary(ModulePath);
    if (pModule == NULL)
      throw Exception_System("LoadLibraryW/dlopen");

    return reinterpret_cast<FuncType>(ImplGetFunctionAddress(pModule, FunctionName));
  }

  template<typename FuncType>
  FuncType Load(void* pModule, std::string const& FunctionName)
  {
    return reinterpret_cast<FuncType>(ImplGetFunctionAddress(pModule, FunctionName));
  }

private:
  typedef std::map< std::wstring, void* > TModuleMap;

  void* ImplLoadLibrary(std::wstring const& LibraryPath);
  void* ImplGetFunctionAddress(void* pModule, std::string const& FunctionName);

  TModuleMap m_ModuleMap;
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_MODULE_
