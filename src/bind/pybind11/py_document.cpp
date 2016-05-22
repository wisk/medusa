#include "py_document.hpp"

#include <pybind11/pybind11.h>

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
}

void PydusaDocument(py::module& rMod)
{
  py::class_<Document>(rMod, "Document")
    .def_property_readonly("memory_areas", pydusa::Document_MemoryAreas)
    .def_property_readonly("labels", pydusa::Document_Labels)
    .def("get_label", &Document::GetLabelFromAddress)
    .def("get_label_address", &Document::GetAddressFromLabelName)
  ;
}
