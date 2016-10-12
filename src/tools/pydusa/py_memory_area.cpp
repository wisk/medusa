#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/memory_area.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaMemoryArea(py::module& rMod)
{
  py::enum_<MemoryArea::Access>(rMod, "MemoryAccess")
    .value("READ",    MemoryArea::Read)
    .value("WRITE",   MemoryArea::Write)
    .value("EXECUTE", MemoryArea::Execute)
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
