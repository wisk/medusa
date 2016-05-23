#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/binary_stream.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  void (FileBinaryStream::*pFileBinaryStream_Open)(boost::filesystem::path const&) = &FileBinaryStream::Open;

  static py::object FileBinaryStream_Read(FileBinaryStream *pBinStrm, unsigned int Offset, size_t Size)
  {
    std::unique_ptr<char[]> upBuffer(new char[Size]);
    if (!pBinStrm->Read(Offset, upBuffer.get(), Size))
      return py::object();
    return py::str(std::string(upBuffer.get(), 0, Size));
  }

  static void MemoryBinaryStream_Open(MemoryBinaryStream *pBinStrm, std::string const& rMem)
  {
    pBinStrm->Open(rMem.data(), rMem.size());
  }

  static py::object MemoryBinaryStream_Read(MemoryBinaryStream *pBinStrm, unsigned int Offset, size_t Size)
  {
    std::unique_ptr<char[]> upBuffer(new char[Size]);
    if (!pBinStrm->Read(Offset, upBuffer.get(), Size))
      return py::object();
    return py::str(std::string(upBuffer.get(), 0, Size));
  }
}

void PydusaBinaryStream(py::module& rMod)
{
  /* Binary stream */

  py::class_<FileBinaryStream>(rMod, "FileBinaryStream")
    .def("open", pydusa::pFileBinaryStream_Open)
    .def("read",     &pydusa::FileBinaryStream_Read)
    .def("close",    &FileBinaryStream::Close)
    .def("__len__",  &FileBinaryStream::GetSize)
    ;

  py::class_<MemoryBinaryStream>(rMod, "MemoryBinaryStream")
    .def("open",    &pydusa::MemoryBinaryStream_Open)
    .def("read",    &pydusa::MemoryBinaryStream_Read)
    .def("close",   &MemoryBinaryStream::Close      )
    .def("__len__", &MemoryBinaryStream::GetSize    )
    ;
}
