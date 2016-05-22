#include "py_memory_area.hpp"

#include <pybind11/pybind11.h>

#include <medusa/memory_area.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  py::object MemoryArea_MoveAddress(MemoryArea const* pMemArea, Address const& rAddress, s64 Offset)
  {
    Address MovedAddr;
    if (!pMemArea->MoveAddress(rAddress, MovedAddr, Offset))
      return py::object();
    return py::cast(MovedAddr);
  }
}

void PydusaMemoryArea(py::module& rMod)
{
  py::enum_<u32>(rMod, "MemoryAccess")
    .value("READ",    MemoryArea::Read)
    .value("WRITE",   MemoryArea::Write)
    .value("EXECUTE", MemoryArea::Execute)
    ;

  py::class_<MemoryArea>(rMod, "MemoryArea")
    .def_property_readonly("name",               &MemoryArea::GetName)
    .def_property_readonly("access",             &MemoryArea::GetAccess)
    .def_property_readonly("base_addr",          &MemoryArea::GetBaseAddress)
    .def_property_readonly("file_offset",           &MemoryArea::GetFileOffset)
    .def_property_readonly("file_size",            &MemoryArea::GetFileSize)
    .def("__len__",                      &MemoryArea::GetSize)
    .def("__str__",                      &MemoryArea::ToString)
    .def("move_address",                 &pydusa::MemoryArea_MoveAddress)
    ;
}
