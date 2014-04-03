#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include "medusa/binary_stream.hpp"

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
, m_FileName("")
, m_FileHandle(-1)
, m_MapHandle()
{
  m_pBuffer = MAP_FAILED;
}

FileBinaryStream::FileBinaryStream(boost::filesystem::path const& rFilePath)
: BinaryStream()
, m_FileName(rFilePath)
, m_FileHandle(-1)
, m_MapHandle()
{
  m_pBuffer = MAP_FAILED;
  Open(rFilePath);
}

FileBinaryStream::~FileBinaryStream(void)
{
  Close();
}

void FileBinaryStream::Open(boost::filesystem::path const& rFilePath)
{
  m_FileName = rFilePath;
  m_FileHandle = open(rFilePath.string().c_str(), O_RDONLY);

  if (m_FileHandle == -1)
    throw Exception_System("open");

  struct stat sb;

  m_MapHandle = NULL;

  if (fstat(m_FileHandle, &sb) == -1)
    throw Exception_System("fstat");

  m_Size = sb.st_size;

  m_pBuffer = mmap(
      NULL,
      m_Size,
      PROT_READ,
      MAP_SHARED,
      m_FileHandle,
      0);

  if (m_pBuffer == MAP_FAILED)
    throw Exception_System("mmap");
}

void FileBinaryStream::Close(void)
{
  if (m_pBuffer != MAP_FAILED)
    munmap(m_pBuffer, m_Size);
  if (m_FileHandle != -1)
    close(m_FileHandle);
  m_Size = 0;
}

/* memory binary stream */

MemoryBinaryStream::MemoryBinaryStream(void)
  : BinaryStream()
{
}

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
  m_pBuffer = ::malloc(MemSize);
  if (m_pBuffer == nullptr)
    throw Exception_System("open");

  m_Size    = MemSize;
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
