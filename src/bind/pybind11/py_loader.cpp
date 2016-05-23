#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/loader.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaLoader(py::module& rMod)
{
  py::class_<Loader>(rMod, "Loader")
    .def_property_readonly("name", &Loader::GetName)
  ;
}
