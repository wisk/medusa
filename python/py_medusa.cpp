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

  Database                & (Medusa::*pMedusa_GetDatabase)(void) =      &Medusa::GetDatabase;
  Architecture::VectorSPtr& (Medusa::*pMedusa_GetArchitectures)(void) = &Medusa::GetArchitectures;
}

void PydusaMedusa(void)
{
  bp::def("Linux_StdOutWorkaround", &pydusa::Linux_StdOutWorkaround);

  bp::class_<Medusa>("Medusa", bp::init<>())
    .def("Open",            &Medusa::Open)
    .def("Close",           &Medusa::Close)
    .def("Load",            &Medusa::Load)
    .def("Save",            &Medusa::Save)
    .def("LoadModules",     &Medusa::LoadModules)
    .def("Disassemble",     &Medusa::Disassemble)
    .add_property("Architectures",
        bp::make_function(
          pydusa::pMedusa_GetArchitectures,
          bp::return_value_policy<bp::reference_existing_object>()))
    .add_property("SupportedLoaders",
        bp::make_function(
          &Medusa::GetSupportedLoaders,
          bp::return_value_policy<bp::reference_existing_object>()))
    .add_property("Serializes",
        bp::make_function(
          &Medusa::GetSerializes,
          bp::return_value_policy<bp::reference_existing_object>()))
    .add_property("Database",
        bp::make_function(
          pydusa::pMedusa_GetDatabase,
          bp::return_value_policy<bp::reference_existing_object>()))
    ;
}