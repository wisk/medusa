#ifndef MEDUSA_BASIC_BLOCK_HPP
#define MEDUSA_BASIC_BLOCK_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/document.hpp"

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT BasicBlockVertexProperties
{
public:
  enum Type
  {
    kIsHead    = 1 << 0,
    kCanReturn = 1 << 1,
    kIsInLoop  = 1 << 2,
  };

  BasicBlockVertexProperties(void);
  BasicBlockVertexProperties(Document const& rDoc, Address::Vector const& rAddresses);

  BasicBlockVertexProperties operator=(BasicBlockVertexProperties const& rBscBlk);

  Address::Vector const& GetAddresses(void) const { return m_Addresses; }
  Address              GetFirstAddress(void) const;
  Address              GetLastAddress(void) const;
  size_t               GetNumberOfInstruction(void) const { return m_Addresses.size(); }
  bool                 Contains(Address const& rAddr) const;
  bool                 Split(Address const& rAddr, Address::Vector& rAddresses);

  bool                 GetPreviousAddress(Address const& rAddr, Address& rPrevAddr) const;
  bool                 GetNextAddress(Address const& rAddr, Address& rNextAddr) const;

  bool                 IsHead(void) const;
  bool                 CanReturn(void) const;
  bool                 IsInLoop(void) const;

private:
  Document const* m_pDoc;
  Address::Vector m_Addresses;
  mutable u32 m_Flags;
  mutable u32 m_TestedFlags;
};

class MEDUSA_EXPORT BasicBlockEdgeProperties
{
public:
  enum Type
  {
    Unknown,
    Unconditional,
    True,
    False,
    Next,
    Multiple,
  };

  BasicBlockEdgeProperties(Type Type = Unknown) : m_Type(Type) {}
  Type GetType(void) const { return m_Type; }

private:
  Type m_Type;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_BASIC_BLOCK_HPP