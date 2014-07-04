#include "py_medusa.hpp"

#include <iostream>

#include <boost/python.hpp>
#include "medusa/medusa.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
}

void PydusaMedusa(void)
{
  bp::class_<Medusa, boost::noncopyable>("Medusa", bp::init<>())
    ;
}
