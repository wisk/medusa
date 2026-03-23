#include "pydusa.hpp"

#include <nanobind/nanobind.h>

#include <medusa/cell.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

void PydusaCell(nb::module_& rMod)
{
  nb::enum_<Cell::Type>(rMod, "CellType")
    .value("INSTRUCTION", Cell::InstructionType)
    .value("VALUE",       Cell::ValueType)
    .value("CHARACTER",   Cell::CharacterType)
    .value("STRING",      Cell::StringType)
    ;

  nb::class_<Cell>(rMod, "Cell")
    .def_prop_ro("type", &Cell::GetType)
    .def("__len__",       &Cell::GetSize)
  ;
}
