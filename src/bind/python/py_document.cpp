#include "py_document.hpp"

#include <boost/python.hpp>

#include <medusa/document.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE;

namespace pydusa
{
  static bp::list Document_ForEachMemoryArea(Document* pDoc)
  {
    bp::list MemAreas;
    pDoc->ForEachMemoryArea([&](MemoryArea const& rMemArea)
    {
      MemAreas.append(boost::ref(rMemArea));
    });
    return MemAreas;
  }
}

void PydusaDocument(void)
{
  bp::class_<Document, boost::noncopyable>("Document", bp::no_init)
  .add_property("memory_areas", pydusa::Document_ForEachMemoryArea)
  ;
}
