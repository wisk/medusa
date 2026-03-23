#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <medusa/document.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE;

namespace pydusa
{
  static nb::list Document_MemoryAreas(Document* pDoc)
  {
    nb::list MemAreas;
    pDoc->ForEachMemoryArea([&](MemoryArea const& rMemArea)
    {
      MemAreas.append(nb::cast(rMemArea));
    });
    return MemAreas;
  }

  static nb::list Document_Labels(Document* pDoc)
  {
    nb::list Labels;
    pDoc->ForEachLabel([&](Address const&, Label const& rLabel)
    {
      Labels.append(nb::cast(rLabel));
    });
    return Labels;
  }

  static nb::object Document_GetLabel(Document* pDoc, Address const& rAddress)
  {
    Label Lbl;
    Lbl = pDoc->GetLabelFromAddress(rAddress);
    if (Lbl.GetType() == Label::Unknown)
      return nb::none();
    return nb::cast(Lbl);
  }

  static nb::object Document_GetLabelAddress(Document* pDoc, std::string const& rLabelName)
  {
    Address Addr;
    Addr = pDoc->GetAddressFromLabelName(rLabelName);
    if (Addr.GetAddressingType() == Address::UnknownType)
      return nb::none();
    return nb::cast(Addr);
  }

  static bool Document_SetAddressArchitectureMode(Document* pDoc, Address const& rAddr, const char* pTagArch, u8 Mode,
                                   Database::SetArchitectureModeType SetArchMode)
  {
    Tag ArchTag = static_cast<Tag>(MEDUSA_MAKE_TAG(pTagArch[0], pTagArch[1], pTagArch[2], 0));

    return pDoc->SetArchitecture(rAddr, ArchTag, Mode, SetArchMode);
  }

  nb::object Document_MoveAddress(Document const* pDocument, Address const& rAddr, s64 Offset)
  {
    Address MovedAddr;
    if (!pDocument->MoveAddress(rAddr, MovedAddr, Offset))
      return nb::none();
    return nb::cast(MovedAddr);
  }

  nb::object Document_GetPreviousAddress(Document const* pDocument, Address const& rAddr)
  {
    Address PrevAddr;
    if (!pDocument->GetPreviousAddress(rAddr, PrevAddr))
      return nb::none();
    return nb::cast(PrevAddr);
  }

  nb::object Document_GetNextAddress(Document const* pDocument, Address const& rAddr)
  {
    Address NextAddr;
    if (!pDocument->GetNextAddress(rAddr, NextAddr))
      return nb::none();
    return nb::cast(NextAddr);
  }

  nb::object Document_GetNearestAddress(Document const* pDocument, Address const& rAddr)
  {
    Address NearestAddr;
    if (!pDocument->GetNearestAddress(rAddr, NearestAddr))
      return nb::none();
    return nb::cast(NearestAddr);
  }
}

void PydusaDocument(nb::module_& rMod)
{
  nb::class_<Document>(rMod, "Document")
    .def_prop_ro("memory_areas", pydusa::Document_MemoryAreas)
    .def_prop_ro("labels", pydusa::Document_Labels)
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

  nb::enum_<Database::SetArchitectureModeType>(rMod, "SetArchMode")
    .value("MEMORY", Database::SetArchitectureModeType::ByMemoryArea)
    .value("CELL", Database::SetArchitectureModeType::ByCell)
    ;
}
