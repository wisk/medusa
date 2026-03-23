#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <medusa/binary_stream.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  void (FileBinaryStream::*pFileBinaryStream_Open)(boost::filesystem::path const&) = &FileBinaryStream::Open;

  static nb::object FileBinaryStream_Read(FileBinaryStream *pBinStrm, unsigned int Offset, size_t Size)
  {
    std::unique_ptr<char[]> upBuffer(new char[Size]);
    if (!pBinStrm->Read(Offset, upBuffer.get(), Size))
      return nb::none();
    return nb::bytes(upBuffer.get(), Size);
  }

  static void MemoryBinaryStream_Open(MemoryBinaryStream *pBinStrm, std::string const& rMem)
  {
    pBinStrm->Open(rMem.data(), rMem.size());
  }

  static nb::object MemoryBinaryStream_Read(MemoryBinaryStream *pBinStrm, unsigned int Offset, size_t Size)
  {
    std::unique_ptr<char[]> upBuffer(new char[Size]);
    if (!pBinStrm->Read(Offset, upBuffer.get(), Size))
      return nb::none();
    return nb::bytes(upBuffer.get(), Size);
  }
}

void PydusaBinaryStream(nb::module_& rMod)
{
  /* Binary stream */

  nb::class_<FileBinaryStream>(rMod, "FileBinaryStream")
    .def("open", pydusa::pFileBinaryStream_Open)
    .def("read",     &pydusa::FileBinaryStream_Read)
    .def("close",    &FileBinaryStream::Close)
    .def("__len__",  &FileBinaryStream::GetSize)
    ;

  nb::class_<MemoryBinaryStream>(rMod, "MemoryBinaryStream")
    .def("open",    &pydusa::MemoryBinaryStream_Open)
    .def("read",    &pydusa::MemoryBinaryStream_Read)
    .def("close",   &MemoryBinaryStream::Close      )
    .def("__len__", &MemoryBinaryStream::GetSize    )
    ;
}
