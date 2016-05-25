#include "py_document.hpp"
#include <boost/python.hpp>
#include <medusa/document.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE;

namespace pydusa
{
  static bp::list Document_MemoryAreas(Document* pDoc)
  {
    bp::list MemAreas;
    pDoc->ForEachMemoryArea([&](MemoryArea const& rMemArea)
    {
      MemAreas.append(boost::ref(rMemArea));
    });
    return MemAreas;
  }

  static bp::list Document_Labels(Document* pDoc)
  {
    bp::list Labels;
    pDoc->ForEachLabel([&](Address const&, Label const& rLabel)
    {
      Labels.append(rLabel);
    });
    return Labels;
  }

  static void   Set_Addr_Arch_Mode(Document* pDoc, Address const& rAddr, bp::str TagArch, u8 Mode,
                                   Document::ESetArchMode Flag)
  {
    const char* s_TagArch = bp::extract<char*>(TagArch);
    Tag arch = static_cast<Tag>(MEDUSA_MAKE_TAG(s_TagArch[0], s_TagArch[1], s_TagArch[2], 0));

    if (Flag == Document::ByMemoryArea) {
      pDoc->SetArchMemoryArea(rAddr, arch, Mode);

    }
    else if (Flag == Document::ByCell) {
      pDoc->SetCellWithArchMode(rAddr, arch, Mode);
    }
  }
}

void PydusaDocument(void)
{
  bp::enum_<Document::ESetArchMode>("ESetArchMode")
          .value("MEMORY", Document::ByMemoryArea)
          .value("CELL", Document::ByCell)
          ;

  bp::class_<Document>("Document")
    .add_property("memory_areas", pydusa::Document_MemoryAreas)
    .add_property("labels", pydusa::Document_Labels)
    .def("set_label", &Document::SetLabelToAddress)
    .def("get_label", &Document::GetLabelFromAddress)
    .def("get_label_addr", &Document::GetAddressFromLabelName)
          .def("set_addr_arch_mode", pydusa::Set_Addr_Arch_Mode)
  ;

  bp::register_ptr_to_python<std::shared_ptr<Document>>();
}
