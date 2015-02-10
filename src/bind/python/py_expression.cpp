#include "py_exception.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <medusa/expression.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaExpression(void)
{
  bp::class_<Expression, boost::noncopyable>("Expression", bp::no_init)
    .def("__str__", &Expression::ToString)
    ;

  bp::register_ptr_to_python<Expression::SPType>();

  bp::class_<Expression::VSPType>("ExprVec")
    .def(bp::vector_indexing_suite<Expression::VSPType>())
    ;
}