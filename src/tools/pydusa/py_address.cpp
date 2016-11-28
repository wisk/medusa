#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/address.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaAddress(py::module& rMod)
{
  auto const& rAddressClass = py::class_<Address>(rMod, "Address")
    .def(py::init<OffsetType>())
    .def(py::init<BaseType, OffsetType>())
    .def(py::init<Address::Type, OffsetType>())
    .def(py::init<Address::Type, BaseType, OffsetType>())

    .def("__str__", &Address::ToString)
    .def_property("base", &Address::GetBase, &Address::SetBase)
    .def_property("offset", &Address::GetOffset, &Address::SetOffset)
  ;

  py::enum_<Address::Type>(rAddressClass, "Type")
    .value("UNKNOWN", Address::UnknownType)
    .value("DEFAULT", Address::DefaultType)
    .value("PHYSICAL", Address::PhysicalType)
    .value("ARCHITECTURE", Address::ArchitectureType)
    .value("LINEAR", Address::LinearType)
    .value("RELATIVE", Address::RelativeType)
    .value("LOGICAL", Address::LogicalType)
    ;
}