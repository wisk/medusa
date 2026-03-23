#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <medusa/information.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  u32 CpuInformation_ConvertNameToIdentifier(CpuInformation const* pCpuInfo, std::string const& rIdName)
  {
    return pCpuInfo->ConvertNameToIdentifier(rIdName);
  }

  nb::object CpuInformation_ConvertIdentifierToName(CpuInformation const* pCpuInfo, u32 IdVal)
  {
    auto pIdName = pCpuInfo->ConvertIdentifierToName(IdVal);
    if (pIdName == nullptr)
      return nb::none();
    return nb::str(pIdName);
  }
}

void PydusaCpuInformation(nb::module_& rMod)
{
  nb::class_<CpuInformation>(rMod, "CpuInformation")
    .def("__getattr__", pydusa::CpuInformation_ConvertNameToIdentifier)
    .def("__getitem__", pydusa::CpuInformation_ConvertIdentifierToName)
    .def("get_identifier_bit_size", &CpuInformation::GetSizeOfRegisterInBit)
  ;
}