#include "medusa/util.hpp"

#include <sstream>
#include <iomanip>
#include <random>

// sha1 / uuid
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/random_generator.hpp>

MEDUSA_NAMESPACE_BEGIN

namespace {
  static constexpr char s_Base64Chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  static unsigned char Base64CharToIndex(unsigned char c)
  {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return 0;
  }
}

std::string Base64Encode(void const *pRawData, u32 Size)
{
  auto pData = reinterpret_cast<u8 const*>(pRawData);
  std::string result;
  result.reserve((Size + 2) / 3 * 4);

  for (u32 i = 0; i < Size; i += 3) {
    u32 n = static_cast<u32>(pData[i]) << 16;
    if (i + 1 < Size) n |= static_cast<u32>(pData[i + 1]) << 8;
    if (i + 2 < Size) n |= static_cast<u32>(pData[i + 2]);

    result += s_Base64Chars[(n >> 18) & 0x3F];
    result += s_Base64Chars[(n >> 12) & 0x3F];
    result += (i + 1 < Size) ? s_Base64Chars[(n >> 6) & 0x3F] : '=';
    result += (i + 2 < Size) ? s_Base64Chars[n & 0x3F] : '=';
  }
  return result;
}

std::string Base64Encode(std::string const &rRawData)
{
  return Base64Encode(rRawData.data(), static_cast<u32>(rRawData.size()));
}

std::string Base64Decode(std::string const &rBase64Data)
{
  std::string result;
  result.reserve(rBase64Data.size() * 3 / 4);

  for (size_t i = 0; i < rBase64Data.size(); ) {
    if (rBase64Data[i] == '=' || rBase64Data[i] == '\0') break;

    u32 a = Base64CharToIndex(static_cast<unsigned char>(rBase64Data[i++]));
    if (i >= rBase64Data.size()) break;
    u32 b = Base64CharToIndex(static_cast<unsigned char>(rBase64Data[i++]));
    result += static_cast<char>((a << 2) | (b >> 4));

    if (i >= rBase64Data.size() || rBase64Data[i] == '=') break;
    u32 c = Base64CharToIndex(static_cast<unsigned char>(rBase64Data[i++]));
    result += static_cast<char>(((b & 0x0F) << 4) | (c >> 2));

    if (i >= rBase64Data.size() || rBase64Data[i] == '=') break;
    u32 d = Base64CharToIndex(static_cast<unsigned char>(rBase64Data[i++]));
    result += static_cast<char>(((c & 0x03) << 6) | d);
  }
  return result;
}


std::string Sha1(void const *pData, size_t Length)
{
  std::ostringstream Result;
  boost::uuids::detail::sha1 Sha1;
  boost::uuids::detail::sha1::digest_type Digest;

  Result << std::hex << std::setfill('0') << std::setw(2);

  Sha1.process_bytes(pData, Length);
  Sha1.get_digest(Digest);
  for (int i = 0; i < 4; ++i)
  {
    auto d = static_cast<uint32_t>(Digest[i]);
    Result << ((d >> 24) & 0xFF);
    Result << ((d >> 16) & 0xFF);
    Result << ((d >>  8) & 0xFF);
    Result << ((d >>  0) & 0xFF);
  }

  return Result.str();
}

Id Sha1(std::string const &rName)
{
  Id Sha1Id;
  boost::uuids::detail::sha1 Sha1;
  boost::uuids::detail::sha1::digest_type Digest;

  Sha1.process_bytes(rName.c_str(), rName.size());
  Sha1.get_digest(Digest);
  for (int i = 0; i < 4; ++i)
  {
    auto d = static_cast<uint32_t>(Digest[i]);
    *(Sha1Id.begin() + i * 4 + 0) = ((d >> 24) & 0xFF);
    *(Sha1Id.begin() + i * 4 + 1) = ((d >> 16) & 0xFF);
    *(Sha1Id.begin() + i * 4 + 2) = ((d >>  8) & 0xFF);
    *(Sha1Id.begin() + i * 4 + 3) = ((d >>  0) & 0xFF);
  }

  return Sha1Id;
}

Id RandomId(void)
{
  static std::mt19937 rng{std::random_device{}()};
  boost::uuids::basic_random_generator<std::mt19937> Gen(rng);
  return Gen();
}

void HexDump(std::ostringstream& rOut, void const* pData, u16 DataLen, Address const& rAddr)
{
  u16 const LineLen = 0x10;
  auto pRaw = reinterpret_cast<u8 const*>(pData);

  Address CurAddr = rAddr;

  for (u16 i = 0; i < DataLen; i += LineLen)
  {
    rOut << CurAddr.ToString() << ": ";

    // Print hex values
    for (u16 j = 0; j < LineLen; ++j)
    {
      if (i + j < DataLen)
        rOut << " " << std::setw(2) << std::setfill('0') << static_cast<unsigned>(pRaw[i + j]);
      else
        rOut << "   ";
    }

    rOut << "  ";

    // Print char if printable
    for (u16 j = 0; j < LineLen; ++j)
    {
      if (i + j < DataLen)
        rOut << (std::isprint(pRaw[i + j]) ? static_cast<char>(pRaw[i + j]) : '.');
      else
        rOut << " ";
    }

    // TODO: it could be wrong (bank, etc)
    CurAddr += LineLen;

    rOut << "\n";
  }
}

MEDUSA_NAMESPACE_END
