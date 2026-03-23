#include "pydusa.hpp"

#include <nanobind/nanobind.h>

#include <medusa/memory_area.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

void PydusaMemoryArea(nb::module_& rMod)
{
  // TODO(wisk): define a custom py::bitmask_
  nb::enum_<MemoryArea::Access>(rMod, "MemoryAccess")
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

  nb::class_<MemoryArea>(rMod, "MemoryArea")
    .def_prop_ro("name",         &MemoryArea::GetName)
    .def_prop_ro("access",       &MemoryArea::GetAccess)
    .def_prop_ro("base_address", &MemoryArea::GetBaseAddress)
    .def_prop_ro("file_offset",  &MemoryArea::GetFileOffset)
    .def_prop_ro("file_size",    &MemoryArea::GetFileSize)
    .def("__len__",                        &MemoryArea::GetSize)
    .def("__str__",                        &MemoryArea::ToString)
    ;
}
