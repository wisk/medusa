#include "pydusa.hpp"

#include <nanobind/nanobind.h>

#include <medusa/architecture.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

void PydusaArchitecture(nb::module_& rMod)
{
  nb::class_<Architecture>(rMod, "Architecture")
    .def_prop_ro("name", &Architecture::GetName)
    .def("translate",      &Architecture::Translate)
    .def("disassemble",    &Architecture::Disassemble)
    .def("get_endianness", &Architecture::GetEndianness)
    .def("format_cell",       &Architecture::FormatCell)
    .def("format_multicell",  &Architecture::FormatMultiCell)
    .def_prop_ro("information", &Architecture::GetCpuInformation, nb::rv_policy::reference_internal)
    ;
}
