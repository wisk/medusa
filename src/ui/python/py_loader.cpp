#include "py_loader.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "medusa/loader.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

template<>
Loader const volatile *boost::get_pointer(Loader const volatile *p)
{
  return p;
}

void PydusaLoader(void)
{
  bp::class_<Loader, boost::noncopyable>("Loader", bp::no_init)
    .add_property("Name",       &Loader::GetName            )
    .def("Map",                 &Loader::Map                )
  ;

  bp::register_ptr_to_python<Loader::SharedPtr>();

  bp::class_<Loader::VectorSharedPtr>("Loaders")
    .def(bp::vector_indexing_suite<Loader::VectorSharedPtr, true>())
    ;
}
