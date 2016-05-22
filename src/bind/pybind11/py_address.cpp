#include "py_address.hpp"

#include <pybind11/pybind11.h>

#include <medusa/address.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaAddress(py::module& rMod)
{
  py::class_<Address>(rMod, "Address")
    .def(py::init<TOffset>())
    .def(py::init<TBase, TOffset>())

    .def("__str__", &Address::ToString)
    .def_property_readonly("base", &Address::GetBase)
    .def_property_readonly("offset", &Address::GetOffset)
  ;
}