#include "pydusa.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <medusa/document.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE;

namespace pydusa
{
  static py::list Document_MemoryAreas(Document* pDoc)
  {
    py::list MemAreas;
    pDoc->ForEachMemoryArea([&](MemoryArea const& rMemArea)
    {
      MemAreas.append(py::cast(rMemArea));
    });
    return MemAreas;
  }

  static py::list Document_Labels(Document* pDoc)
  {
    py::list Labels;
    pDoc->ForEachLabel([&](Address const&, Label const& rLabel)
    {
      Labels.append(py::cast(rLabel));
    });
    return Labels;
  }

  static py::object Document_GetLabel(Document* pDoc, Address const& rAddress)
  {
    Label Lbl;
    Lbl = pDoc->GetLabelFromAddress(rAddress);
    if (Lbl.GetType() == Label::Unknown)
      return py::none();
    return py::cast(Lbl);
  }

  static py::object Document_GetLabelAddress(Document* pDoc, std::string const& rLabelName)
  {
    Address Addr;
    Addr = pDoc->GetAddressFromLabelName(rLabelName);
    if (Addr.GetAddressingType() == Address::UnknownType)
      return py::none();
    return py::cast(Addr);
  }

  static bool Document_SetAddressArchitectureMode(Document* pDoc, Address const& rAddr, const char* pTagArch, u8 Mode,
                                   Database::SetArchitectureModeType SetArchMode)
  {
    Tag ArchTag = static_cast<Tag>(MEDUSA_MAKE_TAG(pTagArch[0], pTagArch[1], pTagArch[2], 0));

    return pDoc->SetArchitecture(rAddr, ArchTag, Mode, SetArchMode);
  }

  py::object Document_MoveAddress(Document const* pDocument, Address const& rAddr, s64 Offset)
  {
    Address MovedAddr;
    if (!pDocument->MoveAddress(rAddr, MovedAddr, Offset))
      return py::none();
    return py::cast(MovedAddr);
  }

  py::object Document_GetPreviousAddress(Document const* pDocument, Address const& rAddr)
  {
    Address PrevAddr;
    if (!pDocument->GetPreviousAddress(rAddr, PrevAddr))
      return py::none();
    return py::cast(PrevAddr);
  }

  py::object Document_GetNextAddress(Document const* pDocument, Address const& rAddr)
  {
    Address NextAddr;
    if (!pDocument->GetNextAddress(rAddr, NextAddr))
      return py::none();
    return py::cast(NextAddr);
  }

  py::object Document_GetNearestAddress(Document const* pDocument, Address const& rAddr)
  {
    Address NearestAddr;
    if (!pDocument->GetNearestAddress(rAddr, NearestAddr))
      return py::none();
    return py::cast(NearestAddr);
  }
}

void PydusaDocument(py::module& rMod)
{
  py::class_<Document, std::shared_ptr<Document>>(rMod, "Document")
    .def_property_readonly("memory_areas", pydusa::Document_MemoryAreas)
    .def_property_readonly("labels", pydusa::Document_Labels)
    .def("get_label", pydusa::Document_GetLabel)
    .def("get_label_address", pydusa::Document_GetLabelAddress)
    .def("set_address_architecture_mode", pydusa::Document_SetAddressArchitectureMode)

    .def("get_architecture_tag", &Document::GetArchitectureTag)
    .def("get_mode", &Document::GetMode)

    .def("move_address", pydusa::Document_MoveAddress)
    .def("get_previous_address", pydusa::Document_GetPreviousAddress)
    .def("get_next_address", pydusa::Document_GetNextAddress)
    .def("get_nearest_address", pydusa::Document_GetNearestAddress)
  ;

  py::enum_<Database::SetArchitectureModeType>(rMod, "SetArchMode")
    .value("MEMORY", Database::SetArchitectureModeType::ByMemoryArea)
    .value("CELL", Database::SetArchitectureModeType::ByCell)
    ;
}
