#include "py_memory_area.hpp"

#include <boost/python.hpp>
#include <boost/python/iterator.hpp>

#include "medusa/memory_area.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

template<>
MemoryArea const volatile *boost::get_pointer(MemoryArea const volatile *p)
{
  return p;
}

namespace pydusa
{
}

void PydusaMemoryArea(void)
{
  bp::enum_<u32>("MemoryAreaType")
    .value("Read",  MemoryArea::Read)
    .value("Write", MemoryArea::Write)
    .value("Execute",  MemoryArea::Execute)
    ;

  bp::class_<MemoryArea, boost::noncopyable>("MemoryArea",  bp::no_init)
    .add_property("Name",               bp::make_function(&MemoryArea::GetName, bp::return_value_policy<bp::copy_const_reference>()))
    .add_property("Access",             &MemoryArea::GetAccess)
    .add_property("BaseAddress",        &MemoryArea::GetBaseAddress)
    .add_property("FileOffset",         &MemoryArea::GetFileOffset)
    .add_property("FileSize",           &MemoryArea::GetFileSize)
    .def("__len__",                     &MemoryArea::GetSize)
    .def("__str__",                     &MemoryArea::ToString)
    ;

  bp::register_ptr_to_python<MemoryArea*>();
}
