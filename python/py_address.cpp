#include "py_address.hpp"

#include <boost/python.hpp>

#include "medusa/address.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaAddress(void)
{
  bp::class_<Address>("Address", bp::init<TBase, TOffset>())
    .def("__str__", &Address::ToString)
    .add_property("Base", &Address::GetBase)
    .add_property("Offset", &Address::GetOffset)
  ;

  bp::register_ptr_to_python<TAddressPtr>();
}
