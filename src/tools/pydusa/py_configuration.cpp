#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/configuration.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

void PydusaConfiguration(py::module& rMod)
{
  py::class_<ConfigurationModel>(rMod, "ConfigurationModel")
    .def("set_bool", &ConfigurationModel::SetBoolean)
    .def("set_enum", &ConfigurationModel::SetEnum)
    .def("is_set",   &ConfigurationModel::IsSet)
    .def("get_bool", &ConfigurationModel::GetBoolean)
    .def("get_enum", &ConfigurationModel::GetEnum)
  ;
}
