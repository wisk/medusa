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
typedef HANDLE            TMapHandle;

MEDUSA_NAMESPACE_END

#else

MEDUSA_NAMESPACE_BEGIN



typedef int               TFileHandle;
typedef void*             TMapHandle;

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
typedef boost::multiprecision::uint128_t  u256;

typedef boost::multiprecision::int512_t   s512;
typedef boost::multiprecision::uint512_t  u512;

typedef boost::multiprecision::int1024_t  s1024;
typedef boost::multiprecision::uint1024_t u1024;

typedef boost::multiprecision::cpp_int    ap_int;


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

#endif // MEDUSA_TYPES_HPP