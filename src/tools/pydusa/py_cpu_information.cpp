#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/information.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  u32 CpuInformation_ConvertNameToIdentifier(CpuInformation const* pCpuInfo, std::string const& rIdName)
  {
    return pCpuInfo->ConvertNameToIdentifier(rIdName);
  }

  py::object CpuInformation_ConvertIdentifierToName(CpuInformation const* pCpuInfo, u32 IdVal)
  {
    auto pIdName = pCpuInfo->ConvertIdentifierToName(IdVal);
    if (pIdName == nullptr)
      return py::none();
    return py::str(pIdName);
  }
}

void PydusaCpuInformation(py::module& rMod)
{
  py::class_<CpuInformation>(rMod, "CpuInformation")
    .def("__getattr__", pydusa::CpuInformation_ConvertNameToIdentifier)
    .def("__getitem__", pydusa::CpuInformation_ConvertIdentifierToName)
  ;
}