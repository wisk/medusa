#include "medusa/basic_block.hpp"
#include "medusa/instruction.hpp"

MEDUSA_NAMESPACE_USE

BasicBlockVertexProperties::BasicBlockVertexProperties(void)
: m_pDoc(nullptr)
, m_Addresses()
, m_Flags()
, m_TestedFlags()
{
}

BasicBlockVertexProperties::BasicBlockVertexProperties(Document const& rDoc, Address::List const& rAddresses)
  : m_pDoc(&rDoc)
  , m_Addresses(rAddresses)
  , m_Flags()
  , m_TestedFlags()
{
  m_Addresses.sort();
}

BasicBlockVertexProperties BasicBlockVertexProperties::operator=(BasicBlockVertexProperties const& rBscBlk)
{
  m_pDoc        = rBscBlk.m_pDoc;
  m_Addresses   = rBscBlk.m_Addresses;
  m_Flags       = rBscBlk.m_Flags;
  m_TestedFlags = rBscBlk.m_TestedFlags;
  return *this;
}

Address BasicBlockVertexProperties::GetFirstAddress(void) const
{
  if (m_Addresses.empty())
    return Address();
  return m_Addresses.front();
}

Address BasicBlockVertexProperties::GetLastAddress(void) const
{
  if (m_Addresses.empty())
    return Address();
  return m_Addresses.back();
}

bool BasicBlockVertexProperties::Contains(Address const& rAddr) const
{
  return std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr) != std::end(m_Addresses);
}

bool BasicBlockVertexProperties::Split(Address const& rAddr, Address::List& rAddresses)
{
  if (GetFirstAddress() == rAddr)
    return false;
  if (m_Addresses.size() < 2)
    return false;

  auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr);
  if (itAddr == std::end(m_Addresses))
    return false;

  rAddresses.splice(std::begin(rAddresses), m_Addresses, itAddr, std::end(m_Addresses));

  return true;
}

bool BasicBlockVertexProperties::GetPreviousAddress(Address const& rAddr, Address& rPrevAddr) const
{
  auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr);
  if (itAddr == std::end(m_Addresses))
    return false;
  if (itAddr == std::begin(m_Addresses))
    return false;
  --itAddr;
  rPrevAddr = *itAddr;
  return true;
}

bool BasicBlockVertexProperties::GetNextAddress(Address const& rAddr, Address& rNextAddr) const
{
  auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr);
  if (itAddr == std::end(m_Addresses))
    return false;
  ++itAddr;
  if (itAddr == std::end(m_Addresses))
    return false;
  rNextAddr = *itAddr;
  return true;
}

bool BasicBlockVertexProperties::IsHead(void) const
{
  if (m_TestedFlags & kIsHead)
    return (m_Flags & kIsHead) ? true : false;
  m_TestedFlags |= kIsHead;

  MultiCell const* pCurFunc = m_pDoc->GetMultiCell(m_Addresses.front());
  if (pCurFunc == nullptr)
    return false;
  if (pCurFunc->GetType() != MultiCell::FunctionType)
    return false;

  m_Flags |= kIsHead;
  return true;
}

bool BasicBlockVertexProperties::CanReturn(void) const
{
  if (m_TestedFlags & kCanReturn)
    return (m_Flags & kCanReturn) ? true : false;
  m_TestedFlags |= kCanReturn;

  auto spInsn = m_pDoc->GetCell(m_Addresses.back());
  if (spInsn == nullptr)
    return false;
  if (!(spInsn->GetSubType() & Instruction::ReturnType))
  return false;

  m_Flags |= kCanReturn;
  return true;
}

bool BasicBlockVertexProperties::IsInLoop(void) const
{
  // TODO: not implemented
  return false;
}