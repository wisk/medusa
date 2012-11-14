#ifndef __MEDUSA_EXTEND_HPP__
#define __MEDUSA_EXTEND_HPP__

#include "medusa/namespace.hpp"

MEDUSA_NAMESPACE_BEGIN

//http://graphics.stanford.edu/~seander/bithacks.html
template<typename _Ty, unsigned _Pos> inline _Ty SignExtend(_Ty const x)
{
  struct { _Ty x : _Pos; } s;
  return s.x = x;
}

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_EXTEND_HPP__