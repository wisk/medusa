#include "pydusa.hpp"

#include <nanobind/nanobind.h>

#include <medusa/address.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

void PydusaAddress(nb::module_& rMod)
{
  auto const& rAddressClass = nb::class_<Address>(rMod, "Address")
    .def(nb::init<>())
    .def(nb::init<OffsetType>())
    .def(nb::init<BaseType, OffsetType>())
    .def(nb::init<Address::Type, OffsetType>())
    .def(nb::init<Address::Type, BaseType, OffsetType>())

    .def("__str__", &Address::ToString)
    .def_prop_rw("base", &Address::GetBase, &Address::SetBase)
    .def_prop_rw("offset", &Address::GetOffset, &Address::SetOffset)
  ;

  nb::enum_<Address::Type>(rAddressClass, "Type")
    .value("UNKNOWN", Address::UnknownType)
    .value("DEFAULT", Address::DefaultType)
    .value("PHYSICAL", Address::PhysicalType)
    .value("ARCHITECTURE", Address::ArchitectureType)
    .value("LINEAR", Address::LinearType)
    .value("RELATIVE", Address::RelativeType)
    .value("LOGICAL", Address::LogicalType)
    ;
}
