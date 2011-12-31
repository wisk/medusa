#include "py_architecture.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "medusa/architecture.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaArchitecture(void)
{
  bp::class_<TInfoMap>("InfoMap")
    .def(bp::map_indexing_suite<TInfoMap, true>())
    ;

  bp::class_<Architecture, boost::noncopyable>("Architecture", bp::no_init)
    .add_property("Name", &Architecture::GetName)
    ;

  bp::register_ptr_to_python<Architecture::Ptr>();

  bp::class_<Architecture::VectorPtr>("Architectures")
    .def(bp::vector_indexing_suite<Architecture::VectorPtr, true>())
    ;
}
