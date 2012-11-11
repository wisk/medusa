#ifndef __MEDUSA_BITS_HPP__
#define __MEDUSA_BITS_HPP__

#include "medusa/namespace.hpp"

#include <boost/static_assert.hpp>

MEDUSA_NAMESPACE_BEGIN

template<unsigned Pos, typename Ty> Ty ExtractBit(Ty Value)
{
  BOOST_STATIC_ASSERT(Pos < (sizeof(Value) * 8));
  return (Value >> Pos) & 1;
}

template<unsigned Beg, unsigned End, typename Ty> Ty ExtractBits(Ty Value)
{
  BOOST_STATIC_ASSERT(Beg < End);
  return (Value >> Beg) & ((1 << (End - Beg)) - 1);
}

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_BITS_HPP__