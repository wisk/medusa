#ifndef MEDUSA_EXTEND_HPP
#define MEDUSA_EXTEND_HPP

#include "medusa/namespace.hpp"

MEDUSA_NAMESPACE_BEGIN

//http://graphics.stanford.edu/~seander/bithacks.html
template<typename _Ty, unsigned _Pos> inline _Ty SignExtend(_Ty const x)
{
  struct { typename std::make_signed<_Ty>::type x : _Pos; } s;
  return s.x = x;
}

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXTEND_HPP