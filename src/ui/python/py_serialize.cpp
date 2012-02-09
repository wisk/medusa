#include "py_serialize.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "medusa/serialize.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaSerialize(void)
{
  bp::class_<Serialize>("Serialize", bp::init<>())
    .add_property("Root", &Serialize::GetRoot)
    .def("Open", &Serialize::Open)
    .def("Load", &Serialize::Load)
    .def("Save", &Serialize::Save)
    ;

  bp::register_ptr_to_python<Serialize::SPtr>();

  bp::class_<Serialize::VectorSPtr>("Serializes")
    .def(bp::vector_indexing_suite<Serialize::VectorSPtr, true>())
    ;
}
