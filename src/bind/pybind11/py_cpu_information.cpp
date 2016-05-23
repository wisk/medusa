#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/information.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaCpuInformation(py::module& rMod)
{
  py::class_<CpuInformation>(rMod, "CpuInformation")
  ;
}