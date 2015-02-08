#include "py_address.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <medusa/address.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaAddress(void)
{
  bp::class_<Address>("Address")
    .def(bp::init<TOffset>())
    .def(bp::init<TBase, TOffset>())

    .def("__str__", &Address::ToString)
    .add_property("base", &Address::GetBase)
    .add_property("offset", &Address::GetOffset)
  ;

  bp::register_ptr_to_python<Address::SPType>();
}