#include "pydusa.hpp"

#include <nanobind/nanobind.h>

#include <medusa/configuration.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

void PydusaConfiguration(nb::module_& rMod)
{
  nb::class_<ConfigurationModel>(rMod, "ConfigurationModel")
    .def("set_bool", &ConfigurationModel::SetBoolean)
    .def("set_enum", &ConfigurationModel::SetEnum)
    .def("is_set",   &ConfigurationModel::IsSet)
    .def("get_bool", &ConfigurationModel::GetBoolean)
    .def("get_enum", &ConfigurationModel::GetEnum)
  ;
}
