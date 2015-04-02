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
inline IntType SignExtend(IntType const& x)
{
  u16 BitSize = x.GetBitSize();
  ap_int InsertedBits = ((ap_int(1) << BitSize) - 1) - ((ap_int(1) << _Pos) - 1);
  ap_int Val = InsertedBits | x.GetValue();
  return IntType(BitSize, Val);
}

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXTEND_HPP