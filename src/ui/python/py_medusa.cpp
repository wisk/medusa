#include "py_medusa.hpp"

#include <iostream>

#include <boost/python.hpp>
#include "medusa/medusa.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  static void Linux_StdOutWorkaround(void)
  {
    std::cout.sync_with_stdio(false);
    std::wcout.sync_with_stdio(false);
  }

  Database                     & (Medusa::*pMedusa_GetDatabase)(void)               = &Medusa::GetDatabase;
  Architecture::VectorSharedPtr& (Medusa::*pMedusa_GetAvailableArchitectures)(void) = &Medusa::GetAvailableArchitectures;
}

void PydusaMedusa(void)
{
  bp::def("Linux_StdOutWorkaround", &pydusa::Linux_StdOutWorkaround);

  bp::class_<Medusa>("Medusa", bp::init<>())
    .def("Open",            &Medusa::Open)
    .def("Close",           &Medusa::Close)
    .def("LoadModules",     &Medusa::LoadModules)
    .def("Disassemble",     &Medusa::Disassemble)
    .add_property("AvailableArchitectures",
        bp::make_function(
          pydusa::pMedusa_GetAvailableArchitectures,
          bp::return_value_policy<bp::reference_existing_object>()))
    .add_property("SupportedLoaders",
        bp::make_function(
          &Medusa::GetSupportedLoaders,
          bp::return_value_policy<bp::reference_existing_object>()))
    .add_property("Database",
        bp::make_function(
          pydusa::pMedusa_GetDatabase,
          bp::return_value_policy<bp::reference_existing_object>()))
    ;
}