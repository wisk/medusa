#ifndef MEDUSA_EXTEND_HPP
#define MEDUSA_EXTEND_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"

MEDUSA_NAMESPACE_BEGIN

//http://graphics.stanford.edu/~seander/bithacks.html
template<typename _Ty, unsigned _Pos> inline _Ty SignExtend(_Ty const x)
{
  struct { typename std::make_signed<_Ty>::type x : _Pos; } s;
  return s.x = x;
}

template<unsigned _Pos>
inline BitVector SignExtend(BitVector const& x)
{
  u16 BitSize = x.GetBitSize();
  auto UnsignedValue = x.GetUnsignedValue();

  if (!((UnsignedValue >> _Pos) & 1))
    return x;

  ap_int InsertedBits = ((ap_int(1) << BitSize) - 1) - ((ap_int(1) << _Pos) - 1);
  UnsignedValue |= InsertedBits;
  return BitVector(BitSize, UnsignedValue);
}

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXTEND_HPP
