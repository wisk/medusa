#include "py_cell.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include "medusa/cell.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaCell(void)
{
  bp::enum_<Cell::Type>("CellType")
    .value("CellType",        Cell::CellType)
    .value("InstructionType", Cell::InstructionType)
    .value("ValueType",       Cell::ValueType)
    .value("CharacterType",   Cell::CharacterType)
    .value("StringType",      Cell::StringType)
    ;

  bp::class_<Cell, Cell::SPType, boost::noncopyable>("Cell", bp::no_init)
    .add_property("type", &Cell::GetType)
    .def("__len__",       &Cell::GetLength)
  ;
}
