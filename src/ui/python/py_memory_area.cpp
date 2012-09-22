#include "py_memory_area.hpp"

#include <boost/python.hpp>
#include <boost/python/iterator.hpp>

#include "medusa/memory_area.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  MemoryArea::TIterator (MemoryArea::*pMemoryArea_Begin)(void)                         = &MemoryArea::Begin;
  MemoryArea::TIterator (MemoryArea::*pMemoryArea_End)(void)                           = &MemoryArea::End;
  bool                  (MemoryArea::*pMemoryArea_IsPresent)(Address::SharedPtr) const = &MemoryArea::IsPresent;
  Cell*                 (MemoryArea::*pMemoryArea_RetrieveCell)(TOffset)               = &MemoryArea::RetrieveCell;

  static bp::str MemoryArea_Read(MemoryArea* pMemoryArea, unsigned int Offset, u32 Size)
  {
    char* pBuffer = new char[Size];
    pMemoryArea->Read(Offset, pBuffer, Size);
    return bp::str(pBuffer, Size);
  }

  static bp::object MemoryArea_GetPhysicalBase(MemoryArea* pMemoryArea)
  {
    Address const& rAddress = pMemoryArea->GetPhysicalBase();

    if (rAddress.GetAddressingType() == Address::UnknownType)
      return bp::object();

    return bp::object(rAddress);
  }

  static bp::object MemoryArea_GetPhysicalSize(MemoryArea* pMemoryArea)
  {
    u32 Size = pMemoryArea->GetPhysicalSize();

    if (Size == 0x0)
      return bp::object();

    return bp::object(Size);
  }

  static bp::object MemoryArea_GetVirtualBase(MemoryArea* pMemoryArea)
  {
    Address const& rAddress = pMemoryArea->GetVirtualBase();

    if (rAddress.GetAddressingType() == Address::UnknownType)
      return bp::object();

    return bp::object(rAddress);
  }

  static bp::object MemoryArea_GetVirtualSize(MemoryArea* pMemoryArea)
  {
    u32 Size = pMemoryArea->GetVirtualSize();

    if (Size == 0x0)
      return bp::object();

    return bp::object(Size);
  }
}

void PydusaMemoryArea(void)
{
  bp::enum_<u32>("MemoryAreaType")
    .value("Read",  MA_READ)
    .value("Write", MA_WRITE)
    .value("Exec",  MA_EXEC)
    ;

  bp::class_<MemoryArea::TCellPair>("CellPair", bp::no_init)
    .add_property("first",  &MemoryArea::TCellPair::first)
    .add_property("second", &MemoryArea::TCellPair::second)
    ;

  bp::class_<MemoryArea>("MemoryArea",  bp::no_init)
    .add_property("Name",               bp::make_function(&MemoryArea::GetName, bp::return_value_policy<bp::copy_const_reference>()))
    .add_property("Access",             &MemoryArea::GetAccess)

    .add_property("PhysicalBase",       &pydusa::MemoryArea_GetPhysicalBase)
    .add_property("PhysicalSize",       &pydusa::MemoryArea_GetPhysicalSize)
    .add_property("VirtualBase",        &pydusa::MemoryArea_GetVirtualBase)
    .add_property("VirtualSize",        &pydusa::MemoryArea_GetVirtualSize)

    .def("Read",                        &pydusa::MemoryArea_Read)
    .def("RetrieveCell",                bp::make_function(pydusa::pMemoryArea_RetrieveCell, bp::return_value_policy<bp::reference_existing_object>()))
    .def("InsertCell",                  &MemoryArea::InsertCell)
    .def("MakeAddress",                 &MemoryArea::MakeAddress)
    .def("__len__",                     &MemoryArea::GetSize)
    .def("__iter__",                    bp::range(pydusa::pMemoryArea_Begin, pydusa::pMemoryArea_End))
    .def("__contains__",                pydusa::pMemoryArea_IsPresent)
    ;

  bp::register_ptr_to_python<MemoryArea*>();
}