#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <medusa/loader.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

void PydusaLoader(nb::module_& rMod)
{
  nb::class_<Loader>(rMod, "Loader")
    .def_prop_ro("name", &Loader::GetName)
    .def_prop_ro("depth", &Loader::GetDepth)
    .def("is_compatible", &Loader::IsCompatible)
    .def("get_detailed_name", &Loader::GetDetailedName)
    .def("get_system_name", &Loader::GetSystemName)
    .def("get_used_architectures", &Loader::GetUsedArchitectures)
    .def("map", nb::overload_cast<Document&>(&Loader::Map, nb::const_))
    .def("analyze", &Loader::Analyze)
  ;
}
