#include "pydusa.hpp"

#include <nanobind/nanobind.h>

#include <medusa/database.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
}

void PydusaDatabase(nb::module_& rMod)
{
  nb::class_<Database>(rMod, "Database")
  ;
}
