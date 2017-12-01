#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/module.hpp>

namespace py = pybind11;

using namespace medusa;

void PydusaModule(py::module& rMod)
{
  rMod.def("get_architecture", [](Tag ArchTag) -> Architecture::SPType
  {
    return ModuleManager::Instance().GetArchitecture(ArchTag);
  });
  rMod.def("get_compiler", [](std::string const& rCompilerName) -> Compiler::SPType
  {
    return ModuleManager::Instance().GetCompiler(rCompilerName);
  });
}