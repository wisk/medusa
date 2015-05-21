#include "medusa/binary_stream.hpp"

#include <Windows.h>

MEDUSA_NAMESPACE_BEGIN

/* binary stream */

BinaryStream::BinaryStream(void)
  : m_pBuffer(nullptr)
  , m_Size(0x0)
  , m_Endianness(EndianUnknown)
{
}

BinaryStream::~BinaryStream(void)
{
}

/* file binary stream */

FileBinaryStream::FileBinaryStream(boost::filesystem::path const& rFilePath)
: BinaryStream()
, m_FileHandle(INVALID_HANDLE_VALUE)
, m_MapHandle(nullptr)
{
  Open(rFilePath);
}

FileBinaryStream::~FileBinaryStream(void)
{
  Close();
}

void FileBinaryStream::Open(boost::filesystem::path const& rFilePath)
{
  if (m_pBuffer != nullptr)
    throw Exception("Binary stream: close the current file first before opening a new one");

  m_Path       = rFilePath;
  m_FileHandle = INVALID_HANDLE_VALUE;
  m_MapHandle  = nullptr;
  m_Endianness = EndianUnknown;

  m_FileHandle = CreateFileW(
      rFilePath.c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr
      );

  if (m_FileHandle == INVALID_HANDLE_VALUE)
    throw Exception_System("CreateFileW");

  LARGE_INTEGER FileSize;

  if (GetFileSizeEx(m_FileHandle, &FileSize) == FALSE)
    throw Exception_System("GetFileSizeEx");

  m_Size = static_cast<u32>(FileSize.QuadPart);

  m_MapHandle = CreateFileMappingW(
      m_FileHandle,
      nullptr,
      PAGE_READONLY,
      FileSize.HighPart,
      FileSize.LowPart,
      nullptr
      );

  if (m_MapHandle == nullptr)
    throw Exception_System("CreateFileMappingW");

  m_pBuffer = MapViewOfFile(
      m_MapHandle,
      FILE_MAP_READ,
      0, 0,
      0
      );

  if (m_pBuffer == nullptr)
    throw Exception_System("MapViewOfFile");
}

void FileBinaryStream::Close(void)
{
  if (m_pBuffer != nullptr)
  {
    UnmapViewOfFile(m_pBuffer);
    m_pBuffer = nullptr;
  }
  if (m_MapHandle != nullptr)
  {
    CloseHandle(m_MapHandle);
    m_MapHandle = nullptr;
  }
  if (m_FileHandle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_FileHandle);
    m_FileHandle = INVALID_HANDLE_VALUE;
  }

  m_Size = 0;
}

/* memory binary stream */

MemoryBinaryStream::MemoryBinaryStream(void const* pMem, u32 MemSize)
  : BinaryStream()
{
  Open(pMem, MemSize);
}

MemoryBinaryStream::~MemoryBinaryStream(void)
{
  Close();
}

void MemoryBinaryStream::Open(void const* pMem, u32 MemSize)
{
  m_Path = boost::filesystem::unique_path();
  m_pBuffer = ::malloc(MemSize);
  if (m_pBuffer == nullptr)
   throw Exception_System("malloc");
  m_Size = MemSize;
  if (pMem != nullptr)
    ::memcpy(m_pBuffer, pMem, MemSize);
}

void MemoryBinaryStream::Close(void)
{
  ::free(m_pBuffer);
  m_pBuffer = nullptr;
  m_Size = 0x0;
  m_Endianness = EndianUnknown;
}

MEDUSA_NAMESPACE_END