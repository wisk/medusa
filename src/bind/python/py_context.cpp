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
    if (!pCpuCtxt->ReadRegister(Reg, RegVal))
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
    case  1: return ReadRegister<bool>(pCpuCtxt, Reg);
    case  8: return ReadRegister<u8  >(pCpuCtxt, Reg);
    case 16: return ReadRegister<u16 >(pCpuCtxt, Reg);
    case 32: return ReadRegister<u32 >(pCpuCtxt, Reg);
    case 64: return ReadRegister<u64 >(pCpuCtxt, Reg);
    default: return bp::object();
    }
  }

  template<typename T>
  void WriteRegister(CpuContext* pCpuCtxt, u32 Reg, bp::object RegVal)
  {
    T Val = bp::extract<T>(RegVal);
    if (!pCpuCtxt->WriteRegister(Reg, Val))
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
    case  1: WriteRegister<bool>(pCpuCtxt, Reg, RegVal); break;
    case  8: WriteRegister<u8  >(pCpuCtxt, Reg, RegVal); break;
    case 16: WriteRegister<u16 >(pCpuCtxt, Reg, RegVal); break;
    case 32: WriteRegister<u32 >(pCpuCtxt, Reg, RegVal); break;
    case 64: WriteRegister<u64 >(pCpuCtxt, Reg, RegVal); break;
    default: Log::Write("pydusa") << "invalid register size " << pRegName << LogEnd;
    }
  }

  static bp::object CpuContext_Translate(CpuContext* pCpuCtxt, Address const& rAddr)
  {
    u64 LinAddr = 0;
    if (!pCpuCtxt->Translate(rAddr, LinAddr))
      return bp::object();
    return bp::object(LinAddr);
  }

  bp::object MemoryContext_ReadBuffer(MemoryContext* pMemCtxt, u64 LinAddr, u32 Size)
  {
    std::unique_ptr<char[]> upBuffer(new char[Size]);
    if (!pMemCtxt->ReadMemory(LinAddr, upBuffer.get(), Size))
      return bp::object();
    return bp::str(upBuffer.get(), Size);
  }

  template<typename _Ty>
  bp::object MemoryContext_Read(MemoryContext* pMemCtxt, u64 LinAddr)
  {
    _Ty Val;
    if (!pMemCtxt->ReadMemory(LinAddr, Val))
      return bp::object();
    return bp::object(Val);
  }

  bp::object MemoryContext_Read_u8(MemoryContext* pMemCtxt, u64 LinAddr)
  { return MemoryContext_Read<u8>(pMemCtxt, LinAddr); }

  bp::object MemoryContext_Read_u16(MemoryContext* pMemCtxt, u64 LinAddr)
  { return MemoryContext_Read<u16>(pMemCtxt, LinAddr); }

  bp::object MemoryContext_Read_u32(MemoryContext* pMemCtxt, u64 LinAddr)
  { return MemoryContext_Read<u32>(pMemCtxt, LinAddr); }

  bp::object MemoryContext_Read_u64(MemoryContext* pMemCtxt, u64 LinAddr)
  { return MemoryContext_Read<u64>(pMemCtxt, LinAddr); }

  bp::object MemoryContext_Read_utf8(MemoryContext* pMemCtxt, u64 LinAddr)
  {
    std::string Res;

    char CurChr = '\0';
    while (pMemCtxt->ReadMemory(LinAddr, CurChr))
    {
      if (CurChr == '\0')
        return bp::str(Res);
      Res += CurChr;
      ++LinAddr;
    }
    return bp::object();
  }
}

void PydusaContext(void)
{
  bp::class_<CpuContext, boost::noncopyable>("CpuContext", bp::no_init)
    .def("__getattr__", pydusa::CpuContext_GetAttr)
    .def("__setattr__", pydusa::CpuContext_SetAttr)
    .def("__str__",     &CpuContext::ToString)
    .def("translate",   pydusa::CpuContext_Translate)
    ;

  bp::class_<MemoryContext, boost::noncopyable>("MemoryContext", bp::no_init)
    .def("__str__",     &MemoryContext::ToString)
    .def("read",        pydusa::MemoryContext_ReadBuffer)
    .def("read_u8",     pydusa::MemoryContext_Read_u8)
    .def("read_u16",    pydusa::MemoryContext_Read_u16)
    .def("read_u32",    pydusa::MemoryContext_Read_u32)
    .def("read_u64",    pydusa::MemoryContext_Read_u64)
    .def("read_utf8",   pydusa::MemoryContext_Read_utf8)
    ;
}
