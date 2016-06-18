#ifndef MEDUSA_UTIL_HPP
#define MEDUSA_UTIL_HPP

#include <medusa/namespace.hpp>
#include <medusa/export.hpp>
#include <medusa/types.hpp>
#include <medusa/address.hpp>
#include <medusa/log.hpp>

#include <string>


MEDUSA_NAMESPACE_BEGIN

// ref: http://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
std::string MEDUSA_EXPORT Base64Encode(void const *pRawData, u32 Size);
std::string MEDUSA_EXPORT Base64Encode(std::string const &rRawData);

std::string MEDUSA_EXPORT Base64Decode(std::string const &rBase64Data);

std::string MEDUSA_EXPORT Sha1(void const *pData, size_t Length);
Id          MEDUSA_EXPORT Sha1(std::string const &Name);

Id          MEDUSA_EXPORT RandomId(void);

void        MEDUSA_EXPORT HexDump(std::ostringstream& rOut, void const* pData, u16 DataLen, Address const& rAddr);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_UTIL_HPP