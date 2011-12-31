#include "medusa/endian.hpp"

MEDUSA_NAMESPACE_BEGIN

bool TestEndian(EEndianness Endianness)
{
#if defined(BOOST_LITTLE_ENDIAN)
  return (Endianness == BigEndian);
#elif defined(BOOST_BIG_ENDIAN)
  return (Endianness == LittleEndian);
#else
# error Unable to determine the current endianness
#endif
}

MEDUSA_NAMESPACE_END
