#ifndef MEDUSA_TYPES_HPP
#define MEDUSA_TYPES_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/filesystem/path.hpp>

#if defined WIN32 || defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__

MEDUSA_NAMESPACE_BEGIN

typedef void*              FileHandleType;
#define INVALID_FILE_VALUE (FileHandleType)-1;
typedef void*              MapHandleType;
#define INVALID_MAP_VALUE  (MapHandleType)nullptr;

MEDUSA_NAMESPACE_END

#else

MEDUSA_NAMESPACE_BEGIN

typedef int               FileHandleType;
#define INVALID_FILE_VALUE (FileHandleType)-1;
typedef void*             MapHandleType;
#define INVALID_MAP_VALUE (FileHandleType*)-1; // MAP_ERROR

MEDUSA_NAMESPACE_END

#endif

#include <stdint.h>
#include <boost/multiprecision/cpp_int.hpp>

MEDUSA_NAMESPACE_BEGIN

typedef  int8_t           s8;
typedef uint8_t           u8;

typedef  int16_t          s16;
typedef uint16_t          u16;

typedef  int32_t          s32;
typedef uint32_t          u32;

typedef  int64_t          s64;
typedef uint64_t          u64;

typedef boost::multiprecision::int128_t   s128;
typedef boost::multiprecision::uint128_t  u128;

typedef boost::multiprecision::int256_t   s256;
typedef boost::multiprecision::uint256_t  u256;

typedef boost::multiprecision::int512_t   s512;
typedef boost::multiprecision::uint512_t  u512;

typedef boost::multiprecision::int1024_t  s1024;
typedef boost::multiprecision::uint1024_t u1024;

typedef boost::multiprecision::number<boost::multiprecision::cpp_int_backend<
  0, 0,
  boost::multiprecision::signed_magnitude,
  boost::multiprecision::unchecked>>
  ap_int;

// boost::multiprecision::cpp_int doesn't support arbitrary unsigned int
typedef ap_int ap_uint;

class MEDUSA_EXPORT BitVector
{
public:
  BitVector(void) : m_BitSize(0), m_Value() {}
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
  typename std::enable_if<std::is_same<_Ty, bool>::value, bool>::type ConvertTo(void) const
  {
    return m_Value.convert_to<u8>() ? true : false;
  }

  template<typename _Ty>
  typename std::enable_if<boost::multiprecision::is_signed_number<_Ty>::value, _Ty>::type ConvertTo(void) const
  {
    return m_Value.convert_to<_Ty>();
  }

  template<typename _Ty>
  typename std::enable_if<boost::multiprecision::is_unsigned_number<_Ty>::value, _Ty>::type ConvertTo(void) const
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

  u16 GetBitSize(void) const { return m_BitSize; }

  void SignExtend(u16 NewBitSize);
  void ZeroExtend(u16 NewBitSize);
  void BitCast(u16 NewBitSize);

  ap_int  GetSignedValue(void) const;
  ap_uint GetUnsignedValue(void) const;

  // Unary

  BitVector  Not(void) const;
  BitVector  operator~(void) const { return Not(); }

  BitVector  Neg(void) const;
  BitVector  operator-(void) const { return Neg(); }

  BitVector& PreInc(void);
  BitVector& operator++(void) { return PreInc(); }
  BitVector  PostInc(void);
  BitVector  operator++(int) { return PostInc(); }

  BitVector& PreDec(void);
  BitVector& operator--(void) { return PreDec(); }
  BitVector  PostDec(void);
  BitVector  operator--(int) { return PostDec(); }

  BitVector  Bsf(void) const;
  BitVector  Lsb(void) const;
  BitVector  Bsr(void) const;
  BitVector  Msb(void) const;

  BitVector  Swap(void) const;

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
  void _Adjust(void);

  u16    m_BitSize;
  ap_int m_Value;
};

template<> float  BitVector::ConvertTo<float> (void) const;
template<> double BitVector::ConvertTo<double>(void) const;

// Address type
typedef u16 BaseType;
typedef u64 OffsetType;
typedef u64 ImageBaseType;

// Architecture
typedef u32 Tag;

// Misc
typedef boost::uuids::uuid      Id;
typedef boost::filesystem::path Path;

MEDUSA_NAMESPACE_END

#define MEDUSA_MAKE_TAG(a,b,c,d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
#define MEDUSA_ARCH_TAG(a,b,c)   MEDUSA_MAKE_TAG(a,b,c,0)
#define MEDUSA_ARCH_UNK          MEDUSA_ARCH_TAG('u', 'n', 'k')
#define MEDUSA_CMP_TAG(lhs, rhs) (((lhs) & 0xffffff00) == ((rhs) & 0xffffff00))

namespace std
{
  template<> struct make_signed<medusa::u128 > { typedef medusa::s128  type; };
  template<> struct make_signed<medusa::u256 > { typedef medusa::s256  type; };
  template<> struct make_signed<medusa::u512 > { typedef medusa::s512  type; };
  template<> struct make_signed<medusa::u1024> { typedef medusa::s1024 type; };
}

#endif // MEDUSA_TYPES_HPP
