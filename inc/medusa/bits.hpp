#ifndef MEDUSA_BITS_HPP
#define MEDUSA_BITS_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"

#include <type_traits>

#ifdef _MSC_VER
# include <intrin.h>
#endif

MEDUSA_NAMESPACE_BEGIN

template<unsigned _Pos, typename _Ty>
_Ty ExtractBit(_Ty Value)
{
  static_assert(_Pos < (sizeof(Value) * 8), "Malformed ExtractBit");
  return (Value >> _Pos) & 1;
}

template<unsigned _Beg, unsigned _End, typename _Ty>
_Ty ExtractBits(_Ty Value)
{
  static_assert(_Beg < _End, "Malformed ExtractBits");
  return (Value >> _Beg) & ((1 << (_End - _Beg + 1)) - 1);
}

template<typename _Ty>
_Ty UnsignedRotateRight(_Ty Value, _Ty Shift)
{
  static_assert(std::is_unsigned<_Ty>::value, "UnsignedRotateRight requires unsigned type");
  return ((Value >> Shift) | (Value << ((sizeof(Value) * 8) - Shift)));
}

// ref: http://stackoverflow.com/questions/355967/how-to-use-msvc-intrinsics-to-get-the-equivalent-of-this-gcc-code
template<typename _Ty>
u8 CountTrailingZero(_Ty Value)
{
#ifdef _MSC_VER
  unsigned long Result = 0;
  return _BitScanForward64(&Result, Value) ? static_cast<u8>(Result) : 32;
#else
  return static_cast<u8>(__builtin_ctzll(Value));
#endif
}

template<typename _Ty>
u8 CountLeadingZero(_Ty Value)
{
#ifdef _MSC_VER
  unsigned long Result = 0;
  return _BitScanReverse64(&Result, Value) ? static_cast<u8>(Result) : 32;
#else
  return static_cast<u8>(__builtin_clzll(Value));
#endif
}

template<typename T = typename std::enable_if<std::is_enum<T>::value, T>::type>
class auto_bool
{
  T val_;
public:
  constexpr auto_bool(T val) : val_(val) {}
  constexpr operator T() const { return val_; }
  constexpr explicit operator bool() const
  {
    return static_cast<std::underlying_type_t<T>>(val_) != 0;
  }
};

template <typename T = typename std::enable_if<std::is_enum<T>::value, T>::type>
constexpr T operator~(T v)
{
  return static_cast<T>(
    ~static_cast<typename std::underlying_type<T>::type>(v));
}

template <typename T = typename std::enable_if<std::is_enum<T>::value, T>::type>
constexpr auto_bool<T> operator&(T lhs, T rhs)
{
  return static_cast<T>(
    static_cast<typename std::underlying_type<T>::type>(lhs) &
    static_cast<typename std::underlying_type<T>::type>(rhs));
}

template <typename T = typename std::enable_if<std::is_enum<T>::value, T>::type>
T& operator&=(T& lhs, T rhs)
{
  auto res = static_cast<T>(
    static_cast<typename std::underlying_type<T>::type>(lhs) |
    static_cast<typename std::underlying_type<T>::type>(rhs));
  lhs = res;
  return lhs;
}

template <typename T = typename std::enable_if<std::is_enum<T>::value, T>::type>
constexpr T operator|(T lhs, T rhs)
{
  return static_cast<T>(
    static_cast<typename std::underlying_type<T>::type>(lhs) |
    static_cast<typename std::underlying_type<T>::type>(rhs));
}

template <typename T = typename std::enable_if<std::is_enum<T>::value, T>::type>
T& operator|=(T& lhs, T rhs)
{
  auto res = static_cast<T>(
    static_cast<typename std::underlying_type<T>::type>(lhs) |
    static_cast<typename std::underlying_type<T>::type>(rhs));
  lhs = res;
  return lhs;
}

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_BITS_HPP