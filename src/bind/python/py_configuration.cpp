#include "py_configuration.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include <medusa/configuration.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaConfiguration(void)
{
  bp::class_<ConfigurationModel>("ConfigurationModel")
    .def("set_bool", &ConfigurationModel::SetBoolean)
    .def("set_enum", &ConfigurationModel::SetEnum)
    .def("is_set",   &ConfigurationModel::IsSet)
    .def("get_bool", &ConfigurationModel::GetBoolean)
    .def("get_enum", &ConfigurationModel::GetEnum)
  ;
}
