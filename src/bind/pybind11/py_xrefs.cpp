#include "py_xrefs.hpp"

#include <pybind11/pybind11.h>

#include <medusa/xref.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  static py::list XRefs_From(XRefs* pXRefs, Address const& rTo)
  {
    Address::List FromList;
    py::list ResList;

    if (pXRefs->From(rTo, FromList))
      for (auto const& rAddr : FromList)
        ResList.append(py::cast(rAddr));

    return ResList;
  }
}

void PydusaXRefs(py::module& rMod)
{
  py::class_<XRefs>(rMod, "XRefs")
    .def("add_xref",  &XRefs::AddXRef    )
    .def("rem_xref",  &XRefs::RemoveRef  )
    .def("from",      &pydusa::XRefs_From)
    .def("to",        &XRefs::To         )
    .def("erase_all", &XRefs::EraseAll   )
  ;
}