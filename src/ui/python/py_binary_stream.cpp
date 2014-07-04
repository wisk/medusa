#include "py_binary_stream.hpp"

#include <Python.h>
#include <boost/python.hpp>

#include "medusa/binary_stream.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  void (FileBinaryStream::*pFileBinaryStream_Open)(boost::filesystem::path const&) = &FileBinaryStream::Open;

  static bp::str FileBinaryStream_Read(FileBinaryStream *pBinStrm, unsigned int Offset, size_t Size)
  {
    char *pBuffer = new char[Size];
    pBinStrm->Read(Offset, pBuffer, Size);
    return bp::str(pBuffer, Size);
  }

  static void MemoryBinaryStream_Open(MemoryBinaryStream *pBinStrm, bp::str s)
  {
    char *pBuf;
    Py_ssize_t Size;

    //PyString_AsStringAndSize(s.ptr(), &pBuf, &Size);
    PyUnicode_AsASCIIString(s.ptr());
    pBinStrm->Open(pBuf, static_cast<u32>(Size));
  }

  static bp::str MemoryBinaryStream_Read(MemoryBinaryStream *pBinStrm, unsigned int Offset, size_t Size)
  {
    char *pBuffer = new char[Size];
    pBinStrm->Read(Offset, pBuffer, Size);
    return bp::str(pBuffer, Size);
  }
}

void PydusaBinaryStream(void)
{
  /* Binary stream */

  bp::class_<FileBinaryStream, boost::noncopyable>("FileBinaryStream", bp::init<>())
    .def("OpenFile", pydusa::pFileBinaryStream_Open)
    .def("Read",     &pydusa::FileBinaryStream_Read)
    .def("Close",    &FileBinaryStream::Close)
    .def("__len__",  &FileBinaryStream::GetSize)
    ;

  bp::class_<MemoryBinaryStream, boost::noncopyable>("MemoryBinaryStream", bp::init<>())
    .def("Open",    &pydusa::MemoryBinaryStream_Open)
    .def("Read",    &pydusa::MemoryBinaryStream_Read)
    .def("Close",   &MemoryBinaryStream::Close      )
    .def("__len__", &MemoryBinaryStream::GetSize    )
    ;
}
