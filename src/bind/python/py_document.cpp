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
}

void PydusaDocument(void)
{
  bp::class_<Document, boost::noncopyable>("Document", bp::no_init)
    .add_property("memory_areas", pydusa::Document_MemoryAreas)
    .add_property("labels", pydusa::Document_Labels)
    .def("get_label", &Document::GetLabelFromAddress)
    .def("get_label_addr", &Document::GetAddressFromLabelName)
  ;
}
