#include "py_cell.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include "medusa/cell.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

template<>
Cell const volatile *boost::get_pointer(Cell const volatile *p)
{
  return p;
}

void PydusaCell(void)
{
  bp::enum_<Cell::Type>("CellType")
    .value("CellType",        Cell::CellType)
    .value("InstructionType", Cell::InstructionType)
    .value("ValueType",       Cell::ValueType)
    .value("CharacterType",   Cell::CharacterType)
    .value("String",          Cell::StringType)
    ;

  bp::class_<Cell, boost::noncopyable>("Cell", bp::no_init)
    .add_property("Type", &Cell::GetType)
    .def("__len__",       &Cell::GetLength)
  ;

  bp::register_ptr_to_python<Cell*>();
}
