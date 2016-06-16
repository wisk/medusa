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
}

void PydusaCpuInformation(py::module& rMod)
{
  py::class_<CpuInformation>(rMod, "CpuInformation")
    .def("__getattr__", pydusa::CpuInformation_ConvertNameToIdentifier)
  ;
}