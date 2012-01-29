#include "py_configuration.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include "medusa/configuration.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaConfiguration(void)
{
  bp::class_<Configuration>("Configuration")
    .def("Get",     &Configuration::Get     )
    .def("IsSet",   &Configuration::IsSet   )
    .def("Set",     &Configuration::Set     )
    .def("Clear",   &Configuration::Clear   )
    .def("__str__", &Configuration::ToString)
  ;

  bp::class_<ConfigurationModel>("ConfigurationModel")
    .add_property("Configuration",
      bp::make_function(
        &ConfigurationModel::GetConfiguration,
        bp::return_value_policy<bp::reference_existing_object>()
    ))
  ;
}