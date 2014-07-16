#include "medusa/util.hpp"

#include <sstream>

// base64
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>

// sha1
#include <boost/uuid/name_generator.hpp>

// random
#include <boost/uuid/random_generator.hpp>
#include <boost/random.hpp>

MEDUSA_NAMESPACE_BEGIN

// ref: http://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
std::string Base64Encode(void const *pRawData, u32 Size)
{
  try
  {
    static const std::string Base64Padding[] = {"", "==","="};
    namespace bai = boost::archive::iterators;
    typedef bai::base64_from_binary<bai::transform_width<char const*, 6, 8>> Base64EncodeType;
    std::stringstream os;

    std::copy(
      Base64EncodeType(pRawData),
      Base64EncodeType(reinterpret_cast<u8 const *>(pRawData) + Size),
      bai::ostream_iterator<char>(os));

    os << Base64Padding[Size % 3];
    return os.str();
  }
  catch (std::exception &e)
  {
    Log::Write("db_text") << "exception: " << e.what() << LogEnd;
  }
  return "";
}

std::string Base64Encode(std::string const &rRawData)
{
  return Base64Encode(rRawData.data(), static_cast<u32>(rRawData.size()));
}

std::string Base64Decode(std::string const &rBase64Data)
{
  std::string Res;

  try
  {
    namespace bai = boost::archive::iterators;
    typedef bai::transform_width<bai::binary_from_base64<const char *>, 8, 6> Base64DecodeType;

    auto const End = rBase64Data.size();
    Base64DecodeType itBase64(rBase64Data.c_str());
    for (std::string::size_type Cur = 0; Cur < End; ++Cur)
    {
      Res += *itBase64;
      ++itBase64;
    }
    return Res;
  }
  // NOTE: we assume we got an exception if the decoding is done (which cannot be always true...)
  catch (std::exception &)
  {
  }
  return Res;
}


std::string Sha1(void const *pData, size_t Length)
{
  std::ostringstream Result;
  boost::uuids::detail::sha1 Sha1;
  unsigned int Digest[5];

  Result << std::hex << std::setfill('0') << std::setw(2);

  Sha1.process_bytes(pData, Length);
  Sha1.get_digest(Digest);
  for (int i = 0; i < 4; ++i)
  {
    Result << ((Digest[i] >> 24) & 0xFF);
    Result << ((Digest[i] >> 16) & 0xFF);
    Result << ((Digest[i] >>  8) & 0xFF);
    Result << ((Digest[i] >>  0) & 0xFF);
  }

  return std::move(Result.str());
}

Id Sha1(std::string const &rName)
{
  Id Sha1Id;
  boost::uuids::detail::sha1 Sha1;
  unsigned int Digest[5];

  Sha1.process_bytes(rName.c_str(), rName.size());
  Sha1.get_digest(Digest);
  for (int i = 0; i < 4; ++i)
  {
    *(Sha1Id.begin() + i * 4 + 0) = ((Digest[i] >> 24) & 0xFF);
    *(Sha1Id.begin() + i * 4 + 1) = ((Digest[i] >> 16) & 0xFF);
    *(Sha1Id.begin() + i * 4 + 2) = ((Digest[i] >>  8) & 0xFF);
    *(Sha1Id.begin() + i * 4 + 3) = ((Digest[i] >>  0) & 0xFF);
  }

  return Sha1Id;
}

Id RandomId(void)
{
  boost::uuids::basic_random_generator<boost::mt19937> Gen;
  return Gen();
}

MEDUSA_NAMESPACE_END