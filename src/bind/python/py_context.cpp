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

  bp::object CpuContext_GetExecuteAddress(CpuContext* pCpuCtxt)
  {
    Address ExecAddr;
    if (!pCpuCtxt->GetAddress(CpuContext::AddressExecution, ExecAddr))
      return bp::object();
    return bp::object(ExecAddr);
  }

  void CpuContext_SetExecuteAddress(CpuContext* pCpuCtxt, Address const& rAddr)
  {
    pCpuCtxt->SetAddress(CpuContext::AddressExecution, rAddr);
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

    // HACK(KS): we have to check for the property here since python first calls this method
    // http://stackoverflow.com/questions/15750522/class-properties-and-setattr
    if (!::strcmp(pRegName, "exec_addr"))
    {
      CpuContext_SetExecuteAddress(pCpuCtxt, bp::extract<Address>(RegVal));
      return;
    }

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

  bp::object MemoryContext_Allocate(MemoryContext* pMemCtxt, u64 LinAddr, u32 Size, u32 Flags)
  {
    void* pRawMem = nullptr;
    if (!pMemCtxt->AllocateMemory(LinAddr, Size, Flags, &pRawMem))
      return bp::object();
    return bp::str(reinterpret_cast<char const*>(pRawMem), Size);
  }

  bp::object MemoryContext_Size(MemoryContext* pMemCtxt, u64 LinAddr)
  {
    u64 Size = 0;
    bool Found = false;
    pMemCtxt->ForEachMemoryChunk([&](MemoryContext::MemoryChunk const& rMemChunk)
    {
      if (LinAddr >= rMemChunk.m_LinearAddress && LinAddr < (rMemChunk.m_LinearAddress + rMemChunk.m_spMemStrm->GetSize()))
      {
        Size = rMemChunk.m_spMemStrm->GetSize();
        Found = true;
        return;
      }
    });

    if (!Found)
      return bp::object();
    return bp::object(Size);
  }

  bp::list MemoryContext_List(MemoryContext* pMemCtxt)
  {
    bp::list MemChunks;
    pMemCtxt->ForEachMemoryChunk([&](MemoryContext::MemoryChunk const& rMemChunk)
    {
      bp::list Tuple;
      Tuple.append(rMemChunk.m_LinearAddress);
      Tuple.append(rMemChunk.m_spMemStrm->GetSize());
      Tuple.append(rMemChunk.m_Flags);
      MemChunks.append(bp::tuple(Tuple));
    });

    return MemChunks;
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

  bool MemoryContext_WriteBuffer(MemoryContext* pMemCtxt, u64 LinAddr, bp::str Buf)
  {
    char *pBuf;
    Py_ssize_t Size;

#if PY_VERSION_HEX < 0x3000000
    PyString_AsStringAndSize(Buf.ptr(), &pBuf, &Size);
#else
    PyBytes_AsStringAndSize(Buf.ptr(), &pBuf, &Size);
#endif
    return pMemCtxt->WriteMemory(LinAddr, pBuf, Size);
  }

  template<typename _Ty>
  bool MemoryContext_Write(MemoryContext* pMemCtxt, u64 LinAddr, _Ty Val)
  { return pMemCtxt->WriteMemory(LinAddr, Val); }

  bool MemoryContext_Write_u8(MemoryContext* pMemCtxt, u64 LinAddr, u8 Val)
  { return MemoryContext_Write<u8>(pMemCtxt, LinAddr, Val); }

  bool MemoryContext_Write_u16(MemoryContext* pMemCtxt, u64 LinAddr, u16 Val)
  { return MemoryContext_Write<u16>(pMemCtxt, LinAddr, Val); }

  bool MemoryContext_Write_u32(MemoryContext* pMemCtxt, u64 LinAddr, u32 Val)
  { return MemoryContext_Write<u32>(pMemCtxt, LinAddr, Val); }

  bool MemoryContext_Write_u64(MemoryContext* pMemCtxt, u64 LinAddr, u64 Val)
  { return MemoryContext_Write<u64>(pMemCtxt, LinAddr, Val); }

  bool MemoryContext_Write_utf8(MemoryContext* pMemCtxt, u64 LinAddr, bp::str Str)
  {
    std::string const& rRawStr = bp::extract<std::string>(Str);
    return pMemCtxt->WriteMemory(LinAddr, rRawStr.c_str(), rRawStr.size() + 1);
  }
}

void PydusaContext(void)
{
  bp::class_<CpuContext, boost::noncopyable>("CpuContext", bp::no_init)
    .def("__getattr__", pydusa::CpuContext_GetAttr)
    .def("__setattr__", pydusa::CpuContext_SetAttr)
    .def("__str__",     &CpuContext::ToString)
    .def("add_mapping", &CpuContext::AddMapping)
    .def("remove_mapping", &CpuContext::RemoveMapping)
    .def("translate",   pydusa::CpuContext_Translate)
    .add_property("info", bp::make_function(&CpuContext::GetCpuInformation, bp::return_internal_reference<>()))
    .add_property("exec_addr",
    pydusa::CpuContext_GetExecuteAddress, pydusa::CpuContext_SetExecuteAddress)
    ;

  bp::class_<MemoryContext, boost::noncopyable>("MemoryContext", bp::no_init)
    .def("__str__",     &MemoryContext::ToString)

    .def("alloc",       pydusa::MemoryContext_Allocate)
    .def("free",        &MemoryContext::FreeMemory)
    .def("size",        pydusa::MemoryContext_Size)

    .def("list",        pydusa::MemoryContext_List)

    .def("read",        pydusa::MemoryContext_ReadBuffer)
    .def("read_u8",     pydusa::MemoryContext_Read_u8)
    .def("read_u16",    pydusa::MemoryContext_Read_u16)
    .def("read_u32",    pydusa::MemoryContext_Read_u32)
    .def("read_u64",    pydusa::MemoryContext_Read_u64)
    .def("read_utf8",   pydusa::MemoryContext_Read_utf8)

    .def("write",       pydusa::MemoryContext_WriteBuffer)
    .def("write_u8",    pydusa::MemoryContext_Write_u8)
    .def("write_u16",   pydusa::MemoryContext_Write_u16)
    .def("write_u32",   pydusa::MemoryContext_Write_u32)
    .def("write_u64",   pydusa::MemoryContext_Write_u64)
    .def("write_utf8",  pydusa::MemoryContext_Write_utf8)
    ;
}
