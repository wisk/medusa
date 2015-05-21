#ifndef MEDUSA_TYPES_HPP
#define MEDUSA_TYPES_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/filesystem/path.hpp>

#if defined WIN32 || defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__

# include <windows.h>

MEDUSA_NAMESPACE_BEGIN

typedef HANDLE            TFileHandle;
#define INVALID_FILE_VALUE (HANDLE)-1;
typedef HANDLE            TMapHandle;
#define INVALID_MAP_VALUE (HANDLE)nullptr;

MEDUSA_NAMESPACE_END

#else

MEDUSA_NAMESPACE_BEGIN

typedef int               TFileHandle;
#define INVALID_FILE_VALUE (int)-1;
typedef void*             TMapHandle;
#define INVALID_MAP_VALUE (void*)-1; // MAP_ERROR

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

class Medusa_EXPORT IntType
{
public:
  IntType(void) : m_BitSize(0), m_Value() {}
  explicit IntType(s8 Value)    : m_BitSize(8),    m_Value(Value) { _Adjust(); }
  explicit IntType(u8 Value)    : m_BitSize(8),    m_Value(Value) { _Adjust(); }
  explicit IntType(s16 Value)   : m_BitSize(16),   m_Value(Value) { _Adjust(); }
  explicit IntType(u16 Value)   : m_BitSize(16),   m_Value(Value) { _Adjust(); }
  explicit IntType(s32 Value)   : m_BitSize(32),   m_Value(Value) { _Adjust(); }
  explicit IntType(u32 Value)   : m_BitSize(32),   m_Value(Value) { _Adjust(); }
  explicit IntType(s64 Value)   : m_BitSize(64),   m_Value(Value) { _Adjust(); }
  explicit IntType(u64 Value)   : m_BitSize(64),   m_Value(Value) { _Adjust(); }
  explicit IntType(s128 Value)  : m_BitSize(128),  m_Value(Value) { _Adjust(); }
  explicit IntType(u128 Value)  : m_BitSize(128),  m_Value(Value) { _Adjust(); }
  explicit IntType(s256 Value)  : m_BitSize(256),  m_Value(Value) { _Adjust(); }
  explicit IntType(u256 Value)  : m_BitSize(256),  m_Value(Value) { _Adjust(); }
  explicit IntType(s512 Value)  : m_BitSize(512),  m_Value(Value) { _Adjust(); }
  explicit IntType(u512 Value)  : m_BitSize(512),  m_Value(Value) { _Adjust(); }
  explicit IntType(s1024 Value) : m_BitSize(1024), m_Value(Value) { _Adjust(); }
  explicit IntType(u1024 Value) : m_BitSize(1024), m_Value(Value) { _Adjust(); }

  explicit IntType(u16 BitSize, ap_int Value) : m_BitSize(BitSize), m_Value(Value) { _Adjust(); }

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

  IntType  Not(void) const;
  IntType  operator~(void) const { return Not(); }

  IntType  Neg(void) const;
  IntType  operator-(void) const { return Neg(); }

  IntType& PreInc(void);
  IntType& operator++(void) { return PreInc(); }
  IntType  PostInc(void);
  IntType  operator++(int) { return PostInc(); }

  IntType& PreDec(void);
  IntType& operator--(void) { return PreDec(); }
  IntType  PostDec(void);
  IntType  operator--(int) { return PostDec(); }

  IntType  Bsf(void) const;
  IntType  Lsb(void) const;
  IntType  Bsr(void) const;
  IntType  Msb(void) const;

  IntType  Swap(void) const;

  // Binary

  IntType  Add(IntType const& rVal) const;
  IntType  operator+(IntType const& rVal) const { return Add(rVal); }
  IntType& AddAssign(IntType const& rVal);
  IntType& operator+=(IntType const& rVal) { return AddAssign(rVal); }

  IntType  Sub(IntType const& rVal) const;
  IntType  operator-(IntType const& rVal) const { return Sub(rVal); }
  IntType& SubAssign(IntType const& rVal);
  IntType& operator-=(IntType const& rVal) { return SubAssign(rVal); }

  IntType  Mul(IntType const& rVal) const;
  IntType  operator*(IntType const& rVal) const { return Mul(rVal); }
  IntType& MulAssign(IntType const& rVal);
  IntType& operator*=(IntType const& rVal) { return MulAssign(rVal); }

  IntType  UDiv(IntType const& rVal) const;
  IntType  operator/(IntType const& rVal) const { return UDiv(rVal); }
  IntType& UDivAssign(IntType const& rVal);
  IntType& operator/=(IntType const& rVal) { return UDivAssign(rVal); }
  IntType  SDiv(IntType const& rVal) const;
  IntType& SDivAssign(IntType const& rVal);

  IntType  UMod(IntType const& rVal) const;
  IntType  operator%(IntType const& rVal) const { return UMod(rVal); }
  IntType& UModAssign(IntType const& rVal);
  IntType& operator%=(IntType const& rVal) { return UModAssign(rVal); }
  IntType  SMod(IntType const& rVal) const;
  IntType& SModAssign(IntType const& rVal);

  IntType  And(IntType const& rVal) const;
  IntType  operator&(IntType const& rVal) const { return And(rVal); }
  IntType& AndAssign(IntType const& rVal);
  IntType& operator&=(IntType const& rVal) { return AndAssign(rVal); }

  IntType Or(IntType const& rVal) const;
  IntType operator|(IntType const& rVal) const { return Or(rVal); }
  IntType& OrAssign(IntType const& rVal);
  IntType& operator|=(IntType const& rVal) { return OrAssign(rVal); }

  IntType  Xor(IntType const& rVal) const;
  IntType  operator^(IntType const& rVal) const { return Xor(rVal); }
  IntType& XorAssign(IntType const& rVal);
  IntType& operator^=(IntType const& rVal) { return XorAssign(rVal); }

  IntType  Lls(IntType const& rVal) const;
  IntType  operator<<(IntType const& rVal) const { return Lls(rVal); }
  IntType& LlsAssign(IntType const& rVal);
  IntType& operator<<=(IntType const& rVal) { return LlsAssign(rVal); }

  IntType  Lrs(IntType const& rVal) const;
  IntType  operator>>(IntType const& rVal) const { return Lrs(rVal); }
  IntType& LrsAssign(IntType const& rVal);
  IntType& operator>>=(IntType const& rVal) { return LrsAssign(rVal); }
  IntType  Ars(IntType const& rVal) const;
  IntType& ArsAssign(IntType const& rVal);

  IntType  Rol(IntType const& rVal) const;
  IntType& RolAssign(IntType const& rVal);
  IntType  Ror(IntType const& rVal) const;
  IntType& RorAssign(IntType const& rVal);

private:
  void _Adjust(void);

  u16 m_BitSize;
  ap_int m_Value;
};

typedef u16                 TTranslationUnit;
typedef u16                 TBase;
typedef u64                 TOffset;
typedef s64                 TRelative;
typedef u64                 TData;
typedef TTranslationUnit    TBank;
typedef TTranslationUnit    TSegment;
typedef u32                 Tag;

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
