#include "py_context.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include <medusa/context.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  template<typename T>
  bp::object ReadRegister(CpuContext* pCpuCtxt, u32 Reg)
  {
    T RegVal;
    if (!pCpuCtxt->ReadRegister(Reg, &RegVal, sizeof(RegVal)))
      return bp::object();
    return bp::object(RegVal);
  }

  static bp::object CpuContext_GetAttr(CpuContext* pCpuCtxt, bp::str RegName)
  {
    char *pRegName;
    Py_ssize_t Size;
    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();

#if PY_VERSION_HEX < 0x3000000
    PyString_AsStringAndSize(RegName.ptr(), &pRegName, &Size);
#else
    PyBytes_AsStringAndSize(RegName.ptr(), &pRegName, &Size);
#endif
    auto Reg = rCpuInfo.ConvertNameToIdentifier(pRegName);
    if (Reg == 0)
      return bp::object();

    auto RegSize = rCpuInfo.GetSizeOfRegisterInBit(Reg);
    switch (RegSize)
    {
    case  8: return ReadRegister<u8 >(pCpuCtxt, Reg);
    case 16: return ReadRegister<u16>(pCpuCtxt, Reg);
    case 32: return ReadRegister<u32>(pCpuCtxt, Reg);
    case 64: return ReadRegister<u64>(pCpuCtxt, Reg);
    default: return bp::object();
    }
  }

  template<typename T>
  void WriteRegister(CpuContext* pCpuCtxt, u32 Reg, bp::object RegVal)
  {
    if (!pCpuCtxt->WriteRegister(Reg, &bp::extract<T>(RegVal), sizeof(T)))
    {
      Log::Write("pydusa") << "unable to write register " << pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(Reg) << LogEnd;
    }
  }

  static void CpuContext_SetAttr(CpuContext* pCpuCtxt, bp::str RegName, bp::object RegVal)
  {
    char *pRegName;
    Py_ssize_t Size;
    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();

#if PY_VERSION_HEX < 0x3000000
    PyString_AsStringAndSize(RegName.ptr(), &pRegName, &Size);
#else
    PyBytes_AsStringAndSize(RegName.ptr(), &pRegName, &Size);
#endif
    auto Reg = rCpuInfo.ConvertNameToIdentifier(pRegName);
    if (Reg == 0)
    {
      Log::Write("pydusa") << "invalid register name " << pRegName << LogEnd;
      return;
    }

    auto RegSize = rCpuInfo.GetSizeOfRegisterInBit(Reg);
    switch (RegSize)
    {
    case  8: WriteRegister< u8>(pCpuCtxt, Reg, RegVal); break;
    case 16: WriteRegister<u16>(pCpuCtxt, Reg, RegVal); break;
    case 32: WriteRegister<u32>(pCpuCtxt, Reg, RegVal); break;
    case 64: WriteRegister<u64>(pCpuCtxt, Reg, RegVal); break;
    default: Log::Write("pydusa") << "invalid register size " << pRegName << LogEnd;
    }
  }
}

void PydusaContext(void)
{
  bp::class_<CpuContext, boost::noncopyable>("CpuContext", bp::no_init)
    .def("__getattr__", pydusa::CpuContext_GetAttr)
    .def("__setattr__", pydusa::CpuContext_SetAttr)
    .def("__str__",     &CpuContext::ToString)
    ;

  bp::class_<MemoryContext, boost::noncopyable>("MemoryContext", bp::no_init)
    .def("__str__",     &MemoryContext::ToString)
    ;
}