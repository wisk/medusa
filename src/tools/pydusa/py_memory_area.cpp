#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/memory_area.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaMemoryArea(py::module& rMod)
{
  // TODO(wisk): define a custom py::bitmask_
  py::enum_<MemoryArea::Access>(rMod, "MemoryAccess")
    .value("READ",    MemoryArea::Access::Read)
    .value("WRITE",   MemoryArea::Access::Write)
    .value("EXECUTE", MemoryArea::Access::Execute)
    .def("__ior__",  [](MemoryArea::Access& lhs, MemoryArea::Access rhs) -> MemoryArea::Access { return lhs = lhs | rhs; })
    .def("__iand__", [](MemoryArea::Access& lhs, MemoryArea::Access rhs) -> MemoryArea::Access { return lhs = lhs & rhs; })
    .def("__ixor__", [](MemoryArea::Access& lhs, MemoryArea::Access rhs) -> MemoryArea::Access { return lhs = lhs ^ rhs; })
    .def("__or__",   [](MemoryArea::Access  lhs, MemoryArea::Access rhs) -> MemoryArea::Access { return lhs | rhs; })
    .def("__and__",  [](MemoryArea::Access  lhs, MemoryArea::Access rhs) -> MemoryArea::Access { return lhs & rhs; })
    .def("__xor__",  [](MemoryArea::Access  lhs, MemoryArea::Access rhs) -> MemoryArea::Access { return lhs ^ rhs; })
    .def("__inv__",  [](MemoryArea::Access  e)                           -> MemoryArea::Access { return ~e; })
    ;

  py::class_<MemoryArea>(rMod, "MemoryArea")
    .def_property_readonly("name",         &MemoryArea::GetName)
    .def_property_readonly("access",       &MemoryArea::GetAccess)
    .def_property_readonly("base_address", &MemoryArea::GetBaseAddress)
    .def_property_readonly("file_offset",  &MemoryArea::GetFileOffset)
    .def_property_readonly("file_size",    &MemoryArea::GetFileSize)
    .def("__len__",                        &MemoryArea::GetSize)
    .def("__str__",                        &MemoryArea::ToString)
    ;
}
