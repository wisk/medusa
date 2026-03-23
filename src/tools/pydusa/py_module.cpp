#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <medusa/module.hpp>

namespace nb = nanobind;

using namespace medusa;

void PydusaModule(nb::module_& rMod)
{
  rMod.def("get_architecture", [](Tag ArchTag) -> Architecture::SPType
  {
    return ModuleManager::Instance().GetArchitecture(ArchTag);
  });
  rMod.def("get_compiler", [](std::string const& rCompilerName) -> Compiler::SPType
  {
    return ModuleManager::Instance().MakeCompiler(rCompilerName);
  });
}