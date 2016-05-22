#include "py_database.hpp"

#include <pybind11/pybind11.h>

#include <medusa/database.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
}

void PydusaDatabase(py::module& rMod)
{
  py::class_<Database>(rMod, "Database")
  ;
}
