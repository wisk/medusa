#include "py_cell.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include "medusa/cell.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaCell(void)
{
  bp::class_<Cell>("Cell",    bp::no_init)
    .add_property("Comment",  bp::make_function(&Cell::GetComment, bp::return_value_policy<bp::copy_const_reference>()))
    .def("__str__",           &Cell::ToString)
    .def("__len__",           &Cell::GetLength)
  ;

  bp::register_ptr_to_python<Cell*>();
}
