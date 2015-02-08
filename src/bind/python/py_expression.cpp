#include "py_exception.hpp"

#include <boost/python.hpp>

#include <medusa/expression.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaExpression(void)
{
  bp::class_<Expression, boost::noncopyable>("Expression", bp::no_init)
    .def("__str__", &Expression::ToString)
    ;

  bp::register_ptr_to_python<Expression::SPType>();
}