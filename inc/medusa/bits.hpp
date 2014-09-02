#ifndef MEDUSA_BITS_HPP
#define MEDUSA_BITS_HPP

#include "medusa/namespace.hpp"

#include <type_traits>

MEDUSA_NAMESPACE_BEGIN

template<unsigned Pos, typename Ty> Ty ExtractBit(Ty Value)
{
  static_assert(Pos < (sizeof(Value) * 8), "Malformed ExtractBit");
  return (Value >> Pos) & 1;
}

template<unsigned Beg, unsigned End, typename Ty> Ty ExtractBits(Ty Value)
{
  static_assert(Beg < End, "Malformed ExtractBits");
  return (Value >> Beg) & ((1 << (End - Beg + 1)) - 1);
}

template<typename Ty> Ty UnsignedRotateRight(Ty Value, Ty Shift)
{
  static_assert(std::is_unsigned<Ty>::value, "UnsignedRotateRight requires unsigned type");
  return ((Value >> Shift) | (Value << ((sizeof(Value) * 8) - Shift)));
}

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_BITS_HPP