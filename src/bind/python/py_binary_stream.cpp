#include "py_binary_stream.hpp"

#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>

#include <medusa/binary_stream.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  void (FileBinaryStream::*pFileBinaryStream_Open)(boost::filesystem::path const&) = &FileBinaryStream::Open;

  static bp::object FileBinaryStream_Read(FileBinaryStream *pBinStrm, unsigned int Offset, size_t Size)
  {
    std::unique_ptr<char[]> upBuffer(new char[Size]);
    if (!pBinStrm->Read(Offset, upBuffer.get(), Size))
      return bp::object();
    return bp::str(upBuffer.get(), Size);
  }

  static void MemoryBinaryStream_Open(MemoryBinaryStream *pBinStrm, bp::str s)
  {
    char *pBuf;
    Py_ssize_t Size;

#if PY_VERSION_HEX < 0x3000000
    PyString_AsStringAndSize(s.ptr(), &pBuf, &Size);
#else
    PyBytes_AsStringAndSize(s.ptr(), &pBuf, &Size);
#endif
    pBinStrm->Open(pBuf, static_cast<u32>(Size));
  }

  static bp::object MemoryBinaryStream_Read(MemoryBinaryStream *pBinStrm, unsigned int Offset, size_t Size)
  {
    std::unique_ptr<char[]> upBuffer(new char[Size]);
    if (!pBinStrm->Read(Offset, upBuffer.get(), Size))
      return bp::object();
    return bp::str(upBuffer.get(), Size);
  }
}

void PydusaBinaryStream(void)
{
  /* Binary stream */

  bp::class_<FileBinaryStream, boost::noncopyable>("FileBinaryStream", bp::init<>())
    .def("open", pydusa::pFileBinaryStream_Open)
    .def("read",     &pydusa::FileBinaryStream_Read)
    .def("close",    &FileBinaryStream::Close)
    .def("__len__",  &FileBinaryStream::GetSize)
    ;

  bp::class_<MemoryBinaryStream, boost::noncopyable>("MemoryBinaryStream", bp::init<>())
    .def("open",    &pydusa::MemoryBinaryStream_Open)
    .def("read",    &pydusa::MemoryBinaryStream_Read)
    .def("close",   &MemoryBinaryStream::Close      )
    .def("__len__", &MemoryBinaryStream::GetSize    )
    ;
}
