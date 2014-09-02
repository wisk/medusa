#ifndef MEDUSA_ENDIAN_HPP
#define MEDUSA_ENDIAN_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include <boost/detail/endian.hpp>

MEDUSA_NAMESPACE_BEGIN

enum EEndianness
{
  EndianUnknown,
  LittleEndian,
  BigEndian
};

//! This function returns true if a swap if needed, otherwise it returns false.
Medusa_EXPORT bool TestEndian(EEndianness Endianness);

//! This function do a swap.
template<typename T> inline void EndianSwap(T&)
{
}

template<> inline void EndianSwap<u16>(u16& rData)
{
  rData = (rData << 8 | rData >> 8);
}

template<> inline void EndianSwap<s16>(s16& rData)
{
  rData = (rData << 8 | rData >> 8);
}

template<> inline void EndianSwap<u32>(u32& rData)
{
  rData =
    (rData >> 24) |
    ((rData << 8) & 0x00FF0000) |
    ((rData >> 8) & 0x0000FF00) |
    (rData << 24);
}

template<> inline void EndianSwap<s32>(s32& rData)
{
  rData =
    (rData >> 24) |
    ((rData << 8) & 0x00FF0000) |
    ((rData >> 8) & 0x0000FF00) |
    (rData << 24);
}

template<> inline void EndianSwap<u64>(u64& rData)
{
  rData = (rData >> 56) |
    ((rData<<40) & 0x00FF000000000000LL) |
    ((rData<<24) & 0x0000FF0000000000LL) |
    ((rData<<8)  & 0x000000FF00000000LL) |
    ((rData>>8)  & 0x00000000FF000000LL) |
    ((rData>>24) & 0x0000000000FF0000LL) |
    ((rData>>40) & 0x000000000000FF00LL) |
    (rData << 56);
}

template<> inline void EndianSwap<s64>(s64& rData)
{
  rData = (rData >> 56) |
    ((rData<<40) & 0x00FF000000000000LL) |
    ((rData<<24) & 0x0000FF0000000000LL) |
    ((rData<<8)  & 0x000000FF00000000LL) |
    ((rData>>8)  & 0x00000000FF000000LL) |
    ((rData>>24) & 0x0000000000FF0000LL) |
    ((rData>>40) & 0x000000000000FF00LL) |
    (rData << 56);
}

MEDUSA_NAMESPACE_END

#endif // MEDUSA_ENDIAN_HPP
