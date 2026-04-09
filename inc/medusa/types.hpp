#ifndef MEDUSA_TYPES_HPP
#define MEDUSA_TYPES_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include <boost/uuid/uuid.hpp>
#include <filesystem>

#if defined WIN32 || defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__

MEDUSA_NAMESPACE_BEGIN

using FileHandleType = void*;
#define INVALID_FILE_VALUE ((FileHandleType)-1)
using MapHandleType = void*;
#define INVALID_MAP_VALUE  ((MapHandleType)nullptr)

MEDUSA_NAMESPACE_END

#else

MEDUSA_NAMESPACE_BEGIN

using FileHandleType = int;
#define INVALID_FILE_VALUE ((FileHandleType)-1)
using MapHandleType = void*;
#define INVALID_MAP_VALUE  ((MapHandleType)-1) // MAP_FAILED

MEDUSA_NAMESPACE_END

#endif

#include <cstdint>
#include <cctype>
#include <boost/multiprecision/cpp_int.hpp>

MEDUSA_NAMESPACE_BEGIN

using s8    = int8_t;
using u8    = uint8_t;

using s16   = int16_t;
using u16   = uint16_t;

using s32   = int32_t;
using u32   = uint32_t;

using s64   = int64_t;
using u64   = uint64_t;

using s128  = boost::multiprecision::int128_t;
using u128  = boost::multiprecision::uint128_t;

using s256  = boost::multiprecision::int256_t;
using u256  = boost::multiprecision::uint256_t;

using s512  = boost::multiprecision::int512_t;
using u512  = boost::multiprecision::uint512_t;

using s1024 = boost::multiprecision::int1024_t;
using u1024 = boost::multiprecision::uint1024_t;

using ap_int = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<
  0, 0,
  boost::multiprecision::signed_magnitude,
  boost::multiprecision::unchecked>>;

// boost::multiprecision::cpp_int doesn't support arbitrary unsigned int
using ap_uint = ap_int;

class MEDUSA_EXPORT BitVector
{
public:
  BitVector() : m_BitSize(0), m_Value() {}
  explicit BitVector(s8 Value)     : m_BitSize(8),    m_Value(Value) { _Adjust(); }
  explicit BitVector(u8 Value)     : m_BitSize(8),    m_Value(Value) { _Adjust(); }
  explicit BitVector(s16 Value)    : m_BitSize(16),   m_Value(Value) { _Adjust(); }
  explicit BitVector(u16 Value)    : m_BitSize(16),   m_Value(Value) { _Adjust(); }
  explicit BitVector(s32 Value)    : m_BitSize(32),   m_Value(Value) { _Adjust(); }
  explicit BitVector(u32 Value)    : m_BitSize(32),   m_Value(Value) { _Adjust(); }
  explicit BitVector(s64 Value)    : m_BitSize(64),   m_Value(Value) { _Adjust(); }
  explicit BitVector(u64 Value)    : m_BitSize(64),   m_Value(Value) { _Adjust(); }
  explicit BitVector(s128 Value)   : m_BitSize(128),  m_Value(Value) { _Adjust(); }
  explicit BitVector(u128 Value)   : m_BitSize(128),  m_Value(Value) { _Adjust(); }
  explicit BitVector(s256 Value)   : m_BitSize(256),  m_Value(Value) { _Adjust(); }
  explicit BitVector(u256 Value)   : m_BitSize(256),  m_Value(Value) { _Adjust(); }
  explicit BitVector(s512 Value)   : m_BitSize(512),  m_Value(Value) { _Adjust(); }
  explicit BitVector(u512 Value)   : m_BitSize(512),  m_Value(Value) { _Adjust(); }
  explicit BitVector(s1024 Value)  : m_BitSize(1024), m_Value(Value) { _Adjust(); }
  explicit BitVector(u1024 Value)  : m_BitSize(1024), m_Value(Value) { _Adjust(); }

  explicit BitVector(float Value)  : BitVector(*reinterpret_cast<u32*>(&Value)) {}
  explicit BitVector(double Value) : BitVector(*reinterpret_cast<u64*>(&Value)) {}

  explicit BitVector(u16 BitSize, ap_int Value) : m_BitSize(BitSize), m_Value(Value) { _Adjust(); }

  template<typename _Ty>
  typename std::enable_if<std::is_same<_Ty, bool>::value, bool>::type ConvertTo() const
  {
    return m_Value.convert_to<u8>() != 0;
  }

  template<typename _Ty>
  typename std::enable_if<boost::multiprecision::is_signed_number<_Ty>::value, _Ty>::type ConvertTo() const
  {
    return m_Value.convert_to<_Ty>();
  }

  template<typename _Ty>
  typename std::enable_if<boost::multiprecision::is_unsigned_number<_Ty>::value, _Ty>::type ConvertTo() const
  {

    auto Res = static_cast<_Ty>(m_Value.convert_to<typename std::make_signed<_Ty>::type>());
    if (Res == 0)
      return 0;
    if (m_Value.backend().sign())
    {
      _Ty Mask = 1;
      Mask <<= m_BitSize;
      --Mask;
      auto Msb = boost::multiprecision::msb(m_Value);
      Mask -= ((1 << Msb) - 1);
      Res |= Mask;
    }
    return Res;
  }

  std::string ToString(u16 Base = 16) const;

  u16 GetBitSize() const { return m_BitSize; }

  void SignExtend(u16 NewBitSize);
  void ZeroExtend(u16 NewBitSize);
  void BitCast(u16 NewBitSize);

  ap_int  GetSignedValue() const;
  ap_uint GetUnsignedValue() const;

  // Unary

  BitVector  Not() const;
  BitVector  operator~() const { return Not(); }

  BitVector  Neg() const;
  BitVector  operator-() const { return Neg(); }

  BitVector& PreInc();
  BitVector& operator++() { return PreInc(); }
  BitVector  PostInc();
  BitVector  operator++(int) { return PostInc(); }

  BitVector& PreDec();
  BitVector& operator--() { return PreDec(); }
  BitVector  PostDec();
  BitVector  operator--(int) { return PostDec(); }

  BitVector  Bsf() const;
  BitVector  Lsb() const;
  BitVector  Bsr() const;
  BitVector  Msb() const;

  BitVector  Swap() const;

  // Binary

  BitVector  Add(BitVector const& rVal) const;
  BitVector  operator+(BitVector const& rVal) const { return Add(rVal); }
  BitVector& AddAssign(BitVector const& rVal);
  BitVector& operator+=(BitVector const& rVal) { return AddAssign(rVal); }

  BitVector  Sub(BitVector const& rVal) const;
  BitVector  operator-(BitVector const& rVal) const { return Sub(rVal); }
  BitVector& SubAssign(BitVector const& rVal);
  BitVector& operator-=(BitVector const& rVal) { return SubAssign(rVal); }

  BitVector  Mul(BitVector const& rVal) const;
  BitVector  operator*(BitVector const& rVal) const { return Mul(rVal); }
  BitVector& MulAssign(BitVector const& rVal);
  BitVector& operator*=(BitVector const& rVal) { return MulAssign(rVal); }

  BitVector  UDiv(BitVector const& rVal) const;
  BitVector  operator/(BitVector const& rVal) const { return UDiv(rVal); }
  BitVector& UDivAssign(BitVector const& rVal);
  BitVector& operator/=(BitVector const& rVal) { return UDivAssign(rVal); }
  BitVector  SDiv(BitVector const& rVal) const;
  BitVector& SDivAssign(BitVector const& rVal);

  BitVector  UMod(BitVector const& rVal) const;
  BitVector  operator%(BitVector const& rVal) const { return UMod(rVal); }
  BitVector& UModAssign(BitVector const& rVal);
  BitVector& operator%=(BitVector const& rVal) { return UModAssign(rVal); }
  BitVector  SMod(BitVector const& rVal) const;
  BitVector& SModAssign(BitVector const& rVal);

  BitVector  And(BitVector const& rVal) const;
  BitVector  operator&(BitVector const& rVal) const { return And(rVal); }
  BitVector& AndAssign(BitVector const& rVal);
  BitVector& operator&=(BitVector const& rVal) { return AndAssign(rVal); }

  BitVector Or(BitVector const& rVal) const;
  BitVector operator|(BitVector const& rVal) const { return Or(rVal); }
  BitVector& OrAssign(BitVector const& rVal);
  BitVector& operator|=(BitVector const& rVal) { return OrAssign(rVal); }

  BitVector  Xor(BitVector const& rVal) const;
  BitVector  operator^(BitVector const& rVal) const { return Xor(rVal); }
  BitVector& XorAssign(BitVector const& rVal);
  BitVector& operator^=(BitVector const& rVal) { return XorAssign(rVal); }

  BitVector  Lls(BitVector const& rVal) const;
  BitVector  operator<<(BitVector const& rVal) const { return Lls(rVal); }
  BitVector& LlsAssign(BitVector const& rVal);
  BitVector& operator<<=(BitVector const& rVal) { return LlsAssign(rVal); }

  BitVector  Lrs(BitVector const& rVal) const;
  BitVector  operator>>(BitVector const& rVal) const { return Lrs(rVal); }
  BitVector& LrsAssign(BitVector const& rVal);
  BitVector& operator>>=(BitVector const& rVal) { return LrsAssign(rVal); }
  BitVector  Ars(BitVector const& rVal) const;
  BitVector& ArsAssign(BitVector const& rVal);

  BitVector  Rol(BitVector const& rVal) const;
  BitVector& RolAssign(BitVector const& rVal);
  BitVector  Ror(BitVector const& rVal) const;
  BitVector& RorAssign(BitVector const& rVal);

  BitVector  FAdd(BitVector const& rVal) const;
  BitVector& FAddAssign(BitVector const& rVal);

  BitVector  FSub(BitVector const& rVal) const;
  BitVector& FSubAssign(BitVector const& rVal);

  BitVector  FMul(BitVector const& rVal) const;
  BitVector& FMulAssign(BitVector const& rVal);

  BitVector  FDiv(BitVector const& rVal) const;
  BitVector& FDivAssign(BitVector const& rVal);

  BitVector  FMod(BitVector const& rVal) const;
  BitVector& FModAssign(BitVector const& rVal);

private:
  void _Adjust();

  u16    m_BitSize;
  ap_int m_Value;
};

template<> float  BitVector::ConvertTo<float> () const;
template<> double BitVector::ConvertTo<double>() const;

// Address type
using BaseType     = u16;
using OffsetType   = u64;
using ImageBaseType = u64;

//// Architecture
//class Tag
//{
//public:
//  template<u8 _A, u8 _B, u8 _C, u8 _D>
//  Tag() : m_Value(_A << 24 | _B << 16 | _C << 8 | _D << 0) {}
//
//  operator u32() const
//  {
//    return m_Value;
//  }
//
//  // TODO(wisk): Handle non-printable characters
//  std::string ToString() const
//  {
//    std::string Result;
//    Result += m_Value >> 24;
//    Result += m_Value >> 16 & 0xff;
//    Result += m_Value >> 8 & 0xff;
//    Result += m_Value & 0xff;
//    return Result;
//  }
//
//  bool operator==(Tag const& Other)
//  {
//    return m_Value == Other.m_Value;
//  }
//
//private:
//  u32 m_Value;
//};

using TagType = u32;
using Tag     = u32;
inline constexpr Tag/*Type*/ MakeTag(u8 A, u8 B, u8 C, u8 D)
{
  return A << 24 | B << 16 | C << 8 | D << 0;
}

// Misc
using Id   = boost::uuids::uuid;
using Path = std::filesystem::path;


MEDUSA_NAMESPACE_END

#define MEDUSA_MAKE_TAG(a,b,c,d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
#define MEDUSA_ARCH_TAG(a,b,c)   MEDUSA_MAKE_TAG(a,b,c,0)
#define MEDUSA_ARCH_UNK          MEDUSA_ARCH_TAG('u', 'n', 'k')
#define MEDUSA_CMP_TAG(lhs, rhs) (((lhs) & 0xffffff00) == ((rhs) & 0xffffff00))

namespace std
{
  template<> struct make_signed<medusa::u128 > { using type = medusa::s128;  };
  template<> struct make_signed<medusa::u256 > { using type = medusa::s256;  };
  template<> struct make_signed<medusa::u512 > { using type = medusa::s512;  };
  template<> struct make_signed<medusa::u1024> { using type = medusa::s1024; };
}

#endif // MEDUSA_TYPES_HPP
