#include "medusa/binary_stream.hpp"

#include <Windows.h>

MEDUSA_NAMESPACE_BEGIN

/* binary stream */

BinaryStream::BinaryStream(void)
  : m_pBuffer(NULL)
  , m_Size(0x0)
  , m_Endianness(EndianUnknown)
{
}

BinaryStream::~BinaryStream(void)
{
}

/* file binary stream */

FileBinaryStream::FileBinaryStream(void)
: BinaryStream()
, m_FileName(L"")
, m_FileHandle(INVALID_HANDLE_VALUE)
, m_MapHandle(NULL)
{
}

FileBinaryStream::FileBinaryStream(std::wstring const& rFilePath)
: BinaryStream()
, m_FileName(rFilePath)
, m_FileHandle(INVALID_HANDLE_VALUE)
, m_MapHandle(NULL)
{
  Open(rFilePath);
}

FileBinaryStream::~FileBinaryStream(void)
{
  Close();
}

void FileBinaryStream::Open(std::wstring const& rFilePath)
{
  if (m_pBuffer != NULL)
    throw Exception(L"Binary stream: close the current file first before opening a new one");

  m_FileName   = rFilePath;
  m_FileHandle = INVALID_HANDLE_VALUE;
  m_MapHandle  = NULL;
  m_Endianness = EndianUnknown;

  m_FileHandle = CreateFileW(
      rFilePath.c_str(),
      GENERIC_READ,
      0x0,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL
      );

  if (m_FileHandle == INVALID_HANDLE_VALUE)
    throw Exception_System(L"CreateFileW");

  LARGE_INTEGER FileSize;

  if (GetFileSizeEx(m_FileHandle, &FileSize) == FALSE)
    throw Exception_System(L"GetFileSizeEx");

  m_Size = static_cast<u32>(FileSize.QuadPart);

  m_MapHandle = CreateFileMappingW(
      m_FileHandle,
      NULL,
      PAGE_READONLY,
      FileSize.HighPart,
      FileSize.LowPart,
      NULL
      );

  if (m_MapHandle == NULL)
    throw Exception_System(L"CreateFileMappingW");

  m_pBuffer = MapViewOfFile(
      m_MapHandle,
      FILE_MAP_READ,
      0, 0,
      0
      );

  if (m_pBuffer == NULL)
    throw Exception_System(L"MapViewOfFile");
}

void FileBinaryStream::Close(void)
{
  if (m_pBuffer != NULL)
  {
    UnmapViewOfFile(m_pBuffer);
    m_pBuffer = NULL;
  }
  if (m_MapHandle != NULL)
  {
    CloseHandle(m_MapHandle);
    m_MapHandle = NULL;
  }
  if (m_FileHandle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_FileHandle);
    m_FileHandle = INVALID_HANDLE_VALUE;
  }

  m_Size = 0;
}

/* memory binary stream */

MemoryBinaryStream::MemoryBinaryStream(void)
  : BinaryStream()
{
}

MemoryBinaryStream::MemoryBinaryStream(void* pMem, u32 MemSize)
  : BinaryStream()
{
  Open(pMem, MemSize);
}

MemoryBinaryStream::~MemoryBinaryStream(void)
{
  Close();
}

void MemoryBinaryStream::Open(void* pMem, u32 MemSize)
{
  m_pBuffer = pMem;
  m_Size    = MemSize;
}

void MemoryBinaryStream::Close(void)
{
  delete [] m_pBuffer;
  m_pBuffer = NULL;
  m_Size = 0x0;
  m_Endianness = EndianUnknown;
}

MEDUSA_NAMESPACE_END