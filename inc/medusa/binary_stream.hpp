#ifndef _MEDUSA_BINARY_STREAM_
#define _MEDUSA_BINARY_STREAM_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/endian.hpp"
#include "medusa/exception.hpp"
#include "medusa/export.hpp"

#include <string>
#include <cstring>

#include <boost/type_traits.hpp>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! BinaryStream is a generic class to handle memory access.
class Medusa_EXPORT BinaryStream
{
public:
  BinaryStream(void);
  virtual ~BinaryStream(void);

  //! This method returns the current endianness.
  EEndianness GetEndianness(void) const             { return m_Endianness;        }

  //! This method sets the desired endianness.
  void        SetEndianness(EEndianness Endianness) { m_Endianness = Endianness;  }

  //! This method reads according to the size of rData and performs a swap if needed.
  void Read(TOffset Position, s8  &rData) const
  { ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  void Read(TOffset Position, u8  &rData) const
  { ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  void Read(TOffset Position, s16 &rData) const
  { ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  void Read(TOffset Position, u16 &rData) const
  { ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  void Read(TOffset Position, s32 &rData) const
  { ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  void Read(TOffset Position, u32 &rData) const
  { ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  void Read(TOffset Position, s64 &rData) const
  { ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  void Read(TOffset Position, u64 &rData) const
  { ReadGeneric(Position, rData); }

  template<typename T, size_t N>
  void Read(TOffset Position, T (&rData)[N]) const
  {
    for (size_t i = 0; i < N; ++i)
    {
      Read(Position, rData[i]);
      Position += sizeof(T);
    }
  }

  //! This method reads a buffer, no swap will be performed.
  void Read(TOffset Position, void* pData, size_t Length) const
  {
    Read(Position, static_cast<u8*>(pData), Length);
  }

  //! This method reads a buffer, no swap will be performed.
  void Read(TOffset Position, u8* pData, size_t Length) const
  {
    if (m_pBuffer == nullptr)
      throw Exception(L"binary stream is not opened");

    if (Position + Length < Position || Position + Length > m_Size)
      throw Exception(L"Read: Position overflow");

    u8 const* pDataPosition = reinterpret_cast<u8 const*>(m_pBuffer) + Position;
    memcpy(pData, pDataPosition, Length);
  }

  void Write(TOffset Position, s8  const& rData)
  { WriteGeneric(Position, rData); }
  void Write(TOffset Position, u8  const& rData)
  { WriteGeneric(Position, rData); }

  void Write(TOffset Position, s16 const& rData)
  { WriteGeneric(Position, rData); }
  void Write(TOffset Position, u16 const& rData)
  { WriteGeneric(Position, rData); }

  void Write(TOffset Position, s32 const& rData)
  { WriteGeneric(Position, rData); }
  void Write(TOffset Position, u32 const& rData)
  { WriteGeneric(Position, rData); }

  void Write(TOffset Position, s64 const& rData)
  { WriteGeneric(Position, rData); }
  void Write(TOffset Position, u64 const& rData)
  { WriteGeneric(Position, rData); }

  template<typename T, size_t N>
  void Write(TOffset Position, T const (&rData)[N]) const
  {
    for (size_t i = 0; i < N; ++i)
    {
      Write(Position, rData[i]);
      Position += sizeof(T);
    }
  }

  void Write(TOffset Position, void const* pData, size_t Length)
  {
    Write(Position, static_cast<u8 const*>(pData), Length);
  }

  void Write(TOffset Position, u8 const* pData, size_t Length)
  {
    if (m_pBuffer == nullptr)
      throw Exception(L"binary stream is not opened");

    if (Position + Length < Position || Position + Length > m_Size)
      throw Exception(L"Write: Position overflow");

    u8* pDataPosition = reinterpret_cast<u8*>(m_pBuffer) + Position;
    memcpy(pDataPosition, pData, Length);
  }

  u32         GetSize(void)   const { return m_Size;    }
  void const* GetBuffer(void) const { return m_pBuffer; }

protected:
  template <typename DataType>
  void ReadGeneric(TOffset Position, DataType& rData) const
  {
    if (m_pBuffer == nullptr)
      throw Exception(L"binary stream is not opened");

    if (Position + sizeof(DataType) > m_Size)
      throw Exception(L"Read: Position overflow");

    u8 const* pDataPosition = reinterpret_cast<u8 const*>(m_pBuffer) + Position;

    rData = *reinterpret_cast<DataType const*>(pDataPosition);
    if (TestEndian(m_Endianness))
      EndianSwap(rData);
  }

  template <typename DataType>
  void WriteGeneric(TOffset Position, DataType& rData)
  {
    if (m_pBuffer == nullptr)
      throw Exception(L"binary stream is not opened");

    if (Position + sizeof(DataType) > m_Size)
      throw Exception(L"Write: Position overflow");

    typename boost::remove_const<DataType>::type* pDataPosition
      = reinterpret_cast< typename boost::remove_const<DataType>::type* >(m_pBuffer) + Position;

    *pDataPosition = rData;
    if (TestEndian(m_Endianness))
      EndianSwap(pDataPosition);
  }

  void*         m_pBuffer;
  u32           m_Size;
  EEndianness   m_Endianness;
};

//! FileBinaryStream is a generic class for file access.
class Medusa_EXPORT FileBinaryStream : public BinaryStream
{
public:
  FileBinaryStream(void);
  FileBinaryStream(std::wstring const& rFilePath);
  virtual ~FileBinaryStream(void);

  void Open(std::wstring const& rFilePath);
  void Close(void);

protected:
  std::wstring  m_FileName;
  TFileHandle   m_FileHandle;
  TMapHandle    m_MapHandle;
};

//! MemoryBinaryStream is similar to BinaryStream.
class Medusa_EXPORT MemoryBinaryStream : public BinaryStream
{
public:
  MemoryBinaryStream(void);
  MemoryBinaryStream(void* pMem, u32 MemSize);
  virtual ~MemoryBinaryStream(void);

  void Open(void* pMem, u32 MemSize);
  void Close(void);
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_BINARY_STREAM_