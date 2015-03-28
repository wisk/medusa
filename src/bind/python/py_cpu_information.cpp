#include "py_cpu_information.hpp"

#include <boost/python.hpp>

#include <medusa/information.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaCpuInformation(void)
{
  bp::class_<CpuInformation, boost::noncopyable>("CpuInformation", bp::no_init)
  ;

  bp::register_ptr_to_python<CpuInformation*>();
}