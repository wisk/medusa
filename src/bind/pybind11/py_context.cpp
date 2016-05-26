#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/context.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  template<typename T>
  py::object ReadRegister(CpuContext* pCpuCtxt, u32 Reg)
  {
    T RegVal;
    if (!pCpuCtxt->ReadRegister(Reg, RegVal))
      return py::object();
    return py::cast(RegVal);
  }

  py::object CpuContext_GetExecuteAddress(CpuContext* pCpuCtxt)
  {
    Address ExecAddr;
    if (!pCpuCtxt->GetAddress(CpuContext::AddressExecution, ExecAddr))
      return py::object();
    return py::cast(ExecAddr);
  }

  void CpuContext_SetExecuteAddress(CpuContext* pCpuCtxt, Address const& rAddr)
  {
    pCpuCtxt->SetAddress(CpuContext::AddressExecution, rAddr);
  }

  static py::object CpuContext_GetAttr(CpuContext* pCpuCtxt, std::string const& rAttrName)
  {
    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();

    auto Reg = rCpuInfo.ConvertNameToIdentifier(rAttrName);
    if (Reg == 0)
      return py::object();

    auto RegSize = rCpuInfo.GetSizeOfRegisterInBit(Reg);
    switch (RegSize)
    {
    case  1: return ReadRegister<bool>(pCpuCtxt, Reg);
    case  8: return ReadRegister<u8  >(pCpuCtxt, Reg);
    case 16: return ReadRegister<u16 >(pCpuCtxt, Reg);
    case 32: return ReadRegister<u32 >(pCpuCtxt, Reg);
    case 64: return ReadRegister<u64 >(pCpuCtxt, Reg);
    default: return py::object();
    }
  }

  template<typename T>
  void WriteRegister(CpuContext* pCpuCtxt, u32 Reg, py::object RegVal)
  {
    T Val = RegVal.cast<T>();
    if (!pCpuCtxt->WriteRegister(Reg, Val))
    {
      Log::Write("pydusa") << "unable to write register " << pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(Reg) << LogEnd;
    }
  }

  static void CpuContext_SetAttr(CpuContext* pCpuCtxt, std::string const& rAttrName, py::object RegVal)
  {
    auto const& rCpuInfo = pCpuCtxt->GetCpuInformation();

    // HACK(KS): we have to check for the property here since python first calls this method
    // http://stackoverflow.com/questions/15750522/class-properties-and-setattr
    if (rAttrName == "exec_addr")
    {
      CpuContext_SetExecuteAddress(pCpuCtxt, RegVal.cast<Address>());
      return;
    }

    auto Reg = rCpuInfo.ConvertNameToIdentifier(rAttrName);
    if (Reg == 0)
    {
      Log::Write("pydusa") << "invalid register name " << rAttrName << LogEnd;
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
    default: Log::Write("pydusa") << "invalid register size " << rAttrName << LogEnd;
    }
  }

  static py::object CpuContext_Translate(CpuContext* pCpuCtxt, Address const& rAddr)
  {
    u64 LinAddr = 0;
    if (!pCpuCtxt->Translate(rAddr, LinAddr))
      return py::object();
    return py::cast(LinAddr);
  }

  py::object MemoryContext_Allocate(MemoryContext* pMemCtxt, u64 LinAddr, u32 Size, u32 Flags)
  {
    void* pRawMem = nullptr;
    if (!pMemCtxt->AllocateMemory(LinAddr, Size, Flags, &pRawMem))
      return py::object();
    return py::cast(std::string(reinterpret_cast<char const*>(pRawMem), 0, Size));
  }

  py::object MemoryContext_Size(MemoryContext* pMemCtxt, u64 LinAddr)
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
      return py::object();
    return py::cast(Size);
  }

  py::list MemoryContext_List(MemoryContext* pMemCtxt)
  {
    py::list MemChunks;
    pMemCtxt->ForEachMemoryChunk([&](MemoryContext::MemoryChunk const& rMemChunk)
    {
      py::list Tuple;
      Tuple.append(py::cast(rMemChunk.m_LinearAddress));
      Tuple.append(py::cast(rMemChunk.m_spMemStrm->GetSize()));
      Tuple.append(py::cast(rMemChunk.m_Flags));
      MemChunks.append(py::tuple(Tuple));
    });

    return MemChunks;
  }

  py::object MemoryContext_ReadBuffer(MemoryContext* pMemCtxt, u64 LinAddr, u32 Size)
  {
    std::unique_ptr<char[]> upBuffer(new char[Size]);
    if (!pMemCtxt->ReadMemory(LinAddr, upBuffer.get(), Size))
      return py::object();
    return py::cast(std::string(upBuffer.get(), 0, Size));
  }

  template<typename _Ty>
  py::object MemoryContext_Read(MemoryContext* pMemCtxt, u64 LinAddr)
  {
    _Ty Val;
    if (!pMemCtxt->ReadMemory(LinAddr, Val))
      return py::object();
    return py::cast(Val);
  }

  py::object MemoryContext_Read_u8(MemoryContext* pMemCtxt, u64 LinAddr)
  { return MemoryContext_Read<u8>(pMemCtxt, LinAddr); }

  py::object MemoryContext_Read_u16(MemoryContext* pMemCtxt, u64 LinAddr)
  { return MemoryContext_Read<u16>(pMemCtxt, LinAddr); }

  py::object MemoryContext_Read_u32(MemoryContext* pMemCtxt, u64 LinAddr)
  { return MemoryContext_Read<u32>(pMemCtxt, LinAddr); }

  py::object MemoryContext_Read_u64(MemoryContext* pMemCtxt, u64 LinAddr)
  { return MemoryContext_Read<u64>(pMemCtxt, LinAddr); }

  py::object MemoryContext_Read_utf8(MemoryContext* pMemCtxt, u64 LinAddr)
  {
    std::string Res;

    char CurChr = '\0';
    while (pMemCtxt->ReadMemory(LinAddr, CurChr))
    {
      if (CurChr == '\0')
        return py::str(Res);
      Res += CurChr;
      ++LinAddr;
    }
    return py::object();
  }

  // TODO(wisk): use a real conversion utf16 to utf8
  py::object MemoryContext_Read_utf16(MemoryContext* pMemCtxt, u64 LinAddr)
  {
    std::string Res;

    char CurChr = '\0';
    while (pMemCtxt->ReadMemory(LinAddr, CurChr))
    {
      if (CurChr == '\0')
        return py::str(Res);
      Res += CurChr;
      LinAddr += 2;
    }
    return py::object();
  }

  bool MemoryContext_WriteBuffer(MemoryContext* pMemCtxt, u64 LinAddr, std::string const& rBuf)
  {
    return pMemCtxt->WriteMemory(LinAddr, rBuf.data(), rBuf.size());
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

  bool MemoryContext_Write_utf8(MemoryContext* pMemCtxt, u64 LinAddr, std::string const& rStr)
  {
    return pMemCtxt->WriteMemory(LinAddr, rStr.c_str(), rStr.size() + 1);
  }
}

void PydusaContext(py::module& rMod)
{
  py::class_<CpuContext>(rMod, "CpuContext")
    .def("__getattr__", pydusa::CpuContext_GetAttr)
    .def("__setattr__", pydusa::CpuContext_SetAttr)
    .def("__str__",     &CpuContext::ToString)
    .def("add_mapping", &CpuContext::AddMapping)
    .def("remove_mapping", &CpuContext::RemoveMapping)
    .def("translate",   pydusa::CpuContext_Translate)
    .def_property_readonly("information", &CpuContext::GetCpuInformation, py::return_value_policy::reference_internal)
    ;

  py::class_<MemoryContext>(rMod, "MemoryContext")
    .def("__str__",     &MemoryContext::ToString)

    .def("allocate",       pydusa::MemoryContext_Allocate)
    .def("free",        &MemoryContext::FreeMemory)
    .def("size",        pydusa::MemoryContext_Size)

    .def("list",        pydusa::MemoryContext_List)

    .def("read",        pydusa::MemoryContext_ReadBuffer)
    .def("read_u8",     pydusa::MemoryContext_Read_u8)
    .def("read_u16",    pydusa::MemoryContext_Read_u16)
    .def("read_u32",    pydusa::MemoryContext_Read_u32)
    .def("read_u64",    pydusa::MemoryContext_Read_u64)
    .def("read_utf8",   pydusa::MemoryContext_Read_utf8)
    .def("read_utf16",  pydusa::MemoryContext_Read_utf16)

    .def("write",       pydusa::MemoryContext_WriteBuffer)
    .def("write_u8",    pydusa::MemoryContext_Write_u8)
    .def("write_u16",   pydusa::MemoryContext_Write_u16)
    .def("write_u32",   pydusa::MemoryContext_Write_u32)
    .def("write_u64",   pydusa::MemoryContext_Write_u64)
    .def("write_utf8",  pydusa::MemoryContext_Write_utf8)
    ;
}
