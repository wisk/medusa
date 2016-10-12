#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/cell.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaCell(py::module& rMod)
{
  py::enum_<Cell::Type>(rMod, "CellType")
    .value("INSTRUCTION", Cell::InstructionType)
    .value("VALUE",       Cell::ValueType)
    .value("CHARACTER",   Cell::CharacterType)
    .value("STRING",      Cell::StringType)
    ;

  py::class_<Cell, Cell::SPType>(rMod, "Cell")
    .def_property_readonly("type", &Cell::GetType)
    .def("__len__",       &Cell::GetSize)
  ;
}
