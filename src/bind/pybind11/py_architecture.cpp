#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/architecture.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaArchitecture(py::module& rMod)
{
  py::class_<Architecture, Architecture::SPType>(rMod, "Architecture")
    .def_property_readonly("name", &Architecture::GetName)
    .def("translate",      &Architecture::Translate)
    .def("disassemble",    &Architecture::Disassemble)
    .def("get_endianness", &Architecture::GetEndianness)
    .def("format_cell",       &Architecture::FormatCell)
    .def("format_multicell",  &Architecture::FormatMultiCell)
    ;
}
