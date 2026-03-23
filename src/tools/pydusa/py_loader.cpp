#include "pydusa.hpp"

#include <nanobind/nanobind.h>

#include <medusa/loader.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

void PydusaLoader(nb::module_& rMod)
{
  nb::class_<Loader>(rMod, "Loader")
    .def_prop_ro("name", &Loader::GetName)
  ;
}
