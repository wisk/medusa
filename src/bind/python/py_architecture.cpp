#include "py_architecture.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <medusa/architecture.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaArchitecture(void)
{
  bp::class_<Architecture, boost::noncopyable>("Architecture", bp::no_init)
    .add_property("name",  &Architecture::GetName)
    .def("translate",      &Architecture::Translate)
    .def("disassemble",    &Architecture::Disassemble)
    .def("get_endianness", &Architecture::GetEndianness)
    .def("fmt_cell",       &Architecture::FormatCell)
    .def("fmt_multicell",  &Architecture::FormatMultiCell)
    ;

  bp::register_ptr_to_python<Architecture::SPType>();

  bp::class_<Architecture::VSPType>("Architectures")
    .def(bp::vector_indexing_suite<Architecture::VSPType, true>())
    ;
}
