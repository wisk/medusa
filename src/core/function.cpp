#include "medusa/function.hpp"
#include <boost/uuid/name_generator.hpp>

MEDUSA_NAMESPACE_BEGIN

Function::Function(MultiCell::Id FunctionId, u16 Size, u16 InsnCnt)
  : MultiCell(FunctionId, MultiCell::FunctionType, Size)
  , m_InsnCnt(InsnCnt)
  , m_ParamCnt()
{}

Function::Function(std::string Name, u16 Size, u16 InsnCnt)
  : MultiCell(MultiCell::Id(), MultiCell::FunctionType, Size)
  , m_InsnCnt(InsnCnt)
  , m_ParamCnt()
{
  boost::uuids::detail::sha1 Sha1;
  unsigned int Digest[5];

  Sha1.process_bytes(Name.c_str(), Name.size());
  Sha1.get_digest(Digest);
  for (int i=0; i<4; ++i) {
    *(m_Id.begin() + i*4+0) = ((Digest[i] >> 24) & 0xFF);
    *(m_Id.begin() + i*4+1) = ((Digest[i] >> 16) & 0xFF);
    *(m_Id.begin() + i*4+2) = ((Digest[i] >> 8) & 0xFF);
    *(m_Id.begin() + i*4+3) = ((Digest[i] >> 0) & 0xFF);
  }
}

MEDUSA_NAMESPACE_END