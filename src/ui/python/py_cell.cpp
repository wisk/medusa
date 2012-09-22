#include "py_cell.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include "medusa/cell.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaCell(void)
{
  bp::enum_<CellInformation::Type>("CellType")
    .value("CellType",        CellInformation::CellType)
    .value("InstructionType", CellInformation::InstructionType)
    .value("ValueType",       CellInformation::ValueType)
    .value("CharacterType",   CellInformation::CharacterType)
    ;

  bp::class_<Cell>("Cell",    bp::no_init)
    .add_property("Type",     &Cell::GetType)
    .add_property("Comment",  bp::make_function
      (&Cell::GetComment, bp::return_value_policy<bp::copy_const_reference>()))
    .def("__str__",           bp::make_function
      (&Cell::ToString, bp::return_value_policy<bp::copy_const_reference>()))
    .def("__len__",           &Cell::GetLength)
  ;

  bp::register_ptr_to_python<Cell*>();
}