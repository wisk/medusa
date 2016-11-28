#ifndef MEDUSA_BINARY_STREAM_HPP
#define MEDUSA_BINARY_STREAM_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/endian.hpp"
#include "medusa/extend.hpp"
#include "medusa/exception.hpp"
#include "medusa/export.hpp"
#include "medusa/util.hpp"

#include <string>
#include <cstring>
#include <memory>

#include <boost/type_traits.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

MEDUSA_NAMESPACE_BEGIN

//! BinaryStream is a generic class to handle memory access.
class MEDUSA_EXPORT BinaryStream
{
public:
  typedef std::shared_ptr<BinaryStream> SPType;

  BinaryStream(void);
  virtual ~BinaryStream(void);

  BinaryStream(BinaryStream const&) = delete;
  BinaryStream& operator=(BinaryStream const&) = delete;

  BinaryStream(BinaryStream&& mBinStrm)
  {
    *this = std::move(mBinStrm);
  }

  BinaryStream& operator=(BinaryStream&& mBinStrm)
  {
    if (this != &mBinStrm)
    {
      m_Path = std::move(mBinStrm.m_Path);

      m_pBuffer = mBinStrm.m_pBuffer;
      mBinStrm.m_pBuffer = nullptr;

      m_Size = mBinStrm.m_Size;
      mBinStrm.m_Size = 0x0;

      m_Endianness = mBinStrm.m_Endianness;
      mBinStrm.m_Endianness = EndianUnknown;

      m_Sha1 = std::move(mBinStrm.m_Sha1);
    }
    return *this;
  }

  virtual void Close(void) = 0;

  //! This method returns a path for the binary stream (for memory binary stream it redirects to a tmp file)
  Path GetPath(void) const { return m_Path; }

  //! This method returns the current endianness.
  EEndianness GetEndianness(void) const { return m_Endianness; }

  //! This method sets the desired endianness.
  void SetEndianness(EEndianness Endianness) { m_Endianness = Endianness;  }

  //! This method reads according to the size of rData and performs a swap if needed.
  bool Read(OffsetType Position, s8  &rData) const
  { return ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  bool Read(OffsetType Position, u8  &rData) const
  { return ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  bool Read(OffsetType Position, s16 &rData) const
  { return ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  bool Read(OffsetType Position, u16 &rData) const
  { return ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  bool Read(OffsetType Position, s32 &rData) const
  { return ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  bool Read(OffsetType Position, u32 &rData) const
  { return ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  bool Read(OffsetType Position, s64 &rData) const
  { return ReadGeneric(Position, rData); }

  //! This method reads according to the size of rData and performs a swap if needed.
  bool Read(OffsetType Position, u64 &rData) const
  { return ReadGeneric(Position, rData); }

  template<typename _Tp>
  bool Read(OffsetType Position, u64& rData, bool SignExtend) const
  {
    _Tp Val;

    if (!Read(Position, Val))
      return false;

    if (SignExtend)
      rData = medusa::SignExtend<s64, sizeof(Val) * 8>(Val);

    return true;
  }

  bool Read(OffsetType Position, u64& rData, u32 Size, bool SignExtend) const
  {
    switch (Size)
    {
    case sizeof(u8):  return Read<u8> (Position, rData, SignExtend);
    case sizeof(u16): return Read<u16>(Position, rData, SignExtend);
    case sizeof(u32): return Read<u32>(Position, rData, SignExtend);
    case sizeof(u64): return Read<u64>(Position, rData, SignExtend);
    default:          return false;
    }
  }

  template<typename T, size_t N>
  bool Read(OffsetType Position, T (&rData)[N]) const
  {
    for (size_t i = 0; i < N; ++i)
    {
      if (Read(Position, rData[i]) == false)
        return false;
      Position += sizeof(T);
    }
    return true;
  }

  u16 StringLength(OffsetType Position) const
  {
    if (Position > m_Size)
      return 0;
    auto Limit = static_cast<OffsetType>(m_Size) - Position; // TODO m_Size should be u64?
    if (Limit == 0)
      return 0;
    char const* pDst = static_cast<char const*>(m_pBuffer) + Position;
    auto StrLen = static_cast<u16>(::strnlen(pDst, Limit));
    if (pDst[StrLen] != '\0')
      return 0;
    return StrLen;
  }

  bool Read(OffsetType Position, std::string& rString) const
  {
    u16 StrLen = StringLength(Position);
    if (StrLen == 0)
      return false;

    char const* pDst = static_cast<char const*>(m_pBuffer) + Position;
    rString.assign(pDst, StrLen);
    return true;
  }

  //! This method reads a buffer, no swap will be performed.
  bool Read(OffsetType Position, void* pData, size_t Length) const
  {
    return Read(Position, static_cast<u8*>(pData), Length);
  }

  //! This method reads a buffer, no swap will be performed.
  bool Read(OffsetType Position, u8* pData, size_t Length) const
  {
    if (m_pBuffer == nullptr)
      return false;

    if (Position + Length < Position || Position + Length > m_Size)
      return false;

    u8 const* pDataPosition = reinterpret_cast<u8 const*>(m_pBuffer) + Position;
    memcpy(pData, pDataPosition, Length);
    return true;
  }

  bool Write(OffsetType Position, s8  const& rData)
  { return WriteGeneric(Position, rData); }
  bool Write(OffsetType Position, u8  const& rData)
  { return WriteGeneric(Position, rData); }

  bool Write(OffsetType Position, s16 const& rData)
  { return WriteGeneric(Position, rData); }
  bool Write(OffsetType Position, u16 const& rData)
  { return WriteGeneric(Position, rData); }

  bool Write(OffsetType Position, s32 const& rData)
  { return WriteGeneric(Position, rData); }
  bool Write(OffsetType Position, u32 const& rData)
  { return WriteGeneric(Position, rData); }

  bool Write(OffsetType Position, s64 const& rData)
  { return WriteGeneric(Position, rData); }
  bool Write(OffsetType Position, u64 const& rData)
  { return WriteGeneric(Position, rData); }

  template<typename T, size_t N>
  bool Write(OffsetType Position, T const (&rData)[N]) const
  {
    for (size_t i = 0; i < N; ++i)
    {
      if (Write(Position, rData[i]) == false)
        return false;
      Position += sizeof(T);
    }

    return true;
  }

  bool Write(OffsetType Position, void const* pData, size_t Length)
  {
    return Write(Position, static_cast<u8 const*>(pData), Length);
  }

  bool Write(OffsetType Position, u8 const* pData, size_t Length)
  {
    if (m_pBuffer == nullptr)
      return false;

    if (Position + Length < Position || Position + Length > m_Size)
      return false;

    u8* pDataPosition = reinterpret_cast<u8*>(m_pBuffer) + Position;
    ::memcpy(pDataPosition, pData, Length);
    return true;
  }

  bool Write(OffsetType Position, int Val, size_t Length)
  {
    if (m_pBuffer == nullptr)
      return false;

    if (Position + Length < Position || Position + Length > m_Size)
      return false;

    u8* pDataPosition = reinterpret_cast<u8*>(m_pBuffer)+Position;
    ::memset(pDataPosition, Val, Length);
    return true;
  }

  u32         GetSize(void)   const { return m_Size;    }
  void*       GetBuffer(void)       { return m_pBuffer; }
  void const* GetBuffer(void) const { return m_pBuffer; }

  std::string const &GetSha1(void) const
  {
    if (m_Sha1.empty())
      m_Sha1 = Sha1(m_pBuffer, m_Size);
    return m_Sha1;
  }

protected:
  template <typename DataType>
  bool ReadGeneric(OffsetType Position, DataType& rData) const
  {
    if (m_pBuffer == nullptr)
      return false;

    if (Position + sizeof(DataType) > m_Size)
      return false;

    u8 const* pDataPosition = reinterpret_cast<u8 const*>(m_pBuffer) + Position;

    rData = *reinterpret_cast<DataType const*>(pDataPosition);
    if (TestEndian(m_Endianness))
      EndianSwap(rData);
    return true;
  }

  template <typename DataType>
  bool WriteGeneric(OffsetType Position, DataType& rData)
  {
    if (m_pBuffer == nullptr)
      return false;

    if (Position + sizeof(DataType) > m_Size)
      return false;

    typename boost::remove_const<DataType>::type* pDataPosition
      = reinterpret_cast< typename boost::remove_const<DataType>::type* >(m_pBuffer) + Position;

    *pDataPosition = rData;
    if (TestEndian(m_Endianness))
      EndianSwap(pDataPosition);
    return true;
  }

  Path                m_Path;
  void*               m_pBuffer;
  u32                 m_Size;
  EEndianness         m_Endianness;
  mutable std::string m_Sha1;
};

//! FileBinaryStream is a generic class for file access.
class MEDUSA_EXPORT FileBinaryStream : public BinaryStream
{
public:
  FileBinaryStream(Path const& rFilePath = Path());
  virtual ~FileBinaryStream(void);

  FileBinaryStream(FileBinaryStream&& mBinStrm)
  {
    *this = std::move(mBinStrm);
  }

  FileBinaryStream& operator=(FileBinaryStream&& mBinStrm)
  {
    if (this != &mBinStrm)
    {
      m_Path = std::move(mBinStrm.m_Path);

      m_pBuffer = mBinStrm.m_pBuffer;
      mBinStrm.m_pBuffer = nullptr;

      m_Size = mBinStrm.m_Size;
      mBinStrm.m_Size = 0x0;

      m_Endianness = mBinStrm.m_Endianness;
      mBinStrm.m_Endianness = EndianUnknown;

      m_Sha1 = std::move(mBinStrm.m_Sha1);

      m_FileHandle = mBinStrm.m_FileHandle;
      mBinStrm.m_FileHandle = INVALID_FILE_VALUE;

      m_MapHandle = mBinStrm.m_MapHandle;
      mBinStrm.m_MapHandle = INVALID_MAP_VALUE;
    }
    return *this;
  }

  void Open(boost::filesystem::path const& rFilePath);
  void Close(void);

protected:
  FileHandleType m_FileHandle;
  MapHandleType  m_MapHandle;
};

//! MemoryBinaryStream handle memory from a raw pointer but its duplicated first.
class MEDUSA_EXPORT MemoryBinaryStream : public BinaryStream
{
public:
  MemoryBinaryStream(void const* pMem = nullptr, u32 MemSize = 0);
  virtual ~MemoryBinaryStream(void);

  MemoryBinaryStream(MemoryBinaryStream&& mBinStrm)
  {
    *this = std::move(mBinStrm);
  }

  MemoryBinaryStream& operator=(MemoryBinaryStream&& mBinStrm)
  {
    if (this != &mBinStrm)
    {
      m_Path = std::move(mBinStrm.m_Path);

      m_pBuffer = mBinStrm.m_pBuffer;
      mBinStrm.m_pBuffer = nullptr;

      m_Size = mBinStrm.m_Size;
      mBinStrm.m_Size = 0x0;

      m_Endianness = mBinStrm.m_Endianness;
      mBinStrm.m_Endianness = EndianUnknown;

      m_Sha1 = std::move(mBinStrm.m_Sha1);
    }
    return *this;
  }

  void Open(void const* pMem, u32 MemSize);
  void Close(void);
};

MEDUSA_NAMESPACE_END

#endif // MEDUSA_BINARY_STREAM_HPP