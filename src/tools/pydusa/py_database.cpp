#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <medusa/database.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

void PydusaDatabase(nb::module_& rMod)
{
  nb::class_<Database>(rMod, "Database")
    .def_prop_ro("name", &Database::GetName)
    .def("open", &Database::Open)
    .def("create", &Database::Create)
    .def("flush", &Database::Flush)
    .def("close", &Database::Close)
  ;
}
