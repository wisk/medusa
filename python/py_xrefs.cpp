#include "py_xrefs.hpp"

#include <boost/foreach.hpp>
#include <boost/utility.hpp>
#include <boost/python.hpp>

#include "medusa/xref.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  static bp::list XRefs_From(XRefs* pXRefs, Address const& rTo)
  {
    Address::List FromList;
    bp::list ResList;

    if (pXRefs->From(rTo, FromList))
      BOOST_FOREACH(Address& rAddr, FromList)
        ResList.append(rAddr);

    return ResList;
  }
}

void PydusaXRefs(void)
{
  bp::class_<XRefs>("XRefs", bp::no_init)
    .def("AddXRef",   &XRefs::AddXRef    )
    .def("RemoveRef", &XRefs::RemoveRef  )
    .def("From",      &pydusa::XRefs_From)
    .def("To",        &XRefs::To         )
    .def("EraseAll",  &XRefs::EraseAll   )
  ;
}