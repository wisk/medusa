#include "py_memory_area.hpp"

#include <boost/python.hpp>
#include <boost/python/iterator.hpp>

#include <medusa/memory_area.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  bp::object MemoryArea_MoveAddress(MemoryArea const* pMemArea, Address const& rAddress, s64 Offset)
  {
    Address MovedAddr;
    if (!pMemArea->MoveAddress(rAddress, MovedAddr, Offset))
      return bp::object();
    return bp::object(MovedAddr);
  }
}

void PydusaMemoryArea(void)
{
  bp::enum_<u32>("MemoryAccess")
    .value("read",  MemoryArea::Read)
    .value("write", MemoryArea::Write)
    .value("exec",  MemoryArea::Execute)
    ;

  bp::class_<MemoryArea, boost::noncopyable>("MemoryArea",  bp::no_init)
    .add_property("name",               bp::make_function(&MemoryArea::GetName, bp::return_value_policy<bp::copy_const_reference>()))
    .add_property("access",             &MemoryArea::GetAccess)
    .add_property("base_addr",          &MemoryArea::GetBaseAddress)
    .add_property("file_off",           &MemoryArea::GetFileOffset)
    .add_property("file_sz",            &MemoryArea::GetFileSize)
    .def("__len__",                     &MemoryArea::GetSize)
    .def("__str__",                     &MemoryArea::ToString)
    .def("move_addr",                   &pydusa::MemoryArea_MoveAddress)
    ;

  bp::register_ptr_to_python<MemoryArea*>();
}
