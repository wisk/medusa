#include "py_configuration.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include "medusa/configuration.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaConfiguration(void)
{
  bp::class_<ConfigurationModel>("ConfigurationModel")
    .def("SetBoolean", &ConfigurationModel::SetBoolean)
    .def("SetEnum", &ConfigurationModel::SetEnum)
    .def("IsSet", &ConfigurationModel::IsSet)
    .def("GetBoolean", &ConfigurationModel::GetBoolean)
    .def("GetEnum", &ConfigurationModel::GetEnum)
  ;
}
