#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/address.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaAddress(py::module& rMod)
{
  py::class_<Address>(rMod, "Address")
    .def(py::init<OffsetType>())
    .def(py::init<BaseType, OffsetType>())

    .def("__str__", &Address::ToString)
    .def_property("base", &Address::GetBase, &Address::SetBase)
    .def_property("offset", &Address::GetOffset, &Address::SetOffset)
  ;
}