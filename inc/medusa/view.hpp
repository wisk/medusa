#ifndef __MEDUSA_VIEW_HPP__
#define __MEDUSA_VIEW_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

#include <map>
#include <set>
#include <iterator>
#include <boost/thread/mutex.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT View
{
public:
  class Medusa_EXPORT LineInformation
  {
  public:
    enum Type
    {
      UnknownLineType,
      CellLineType,
      MultiCellLineType,
      LabelLineType,
      XrefLineType,
      MemoryAreaLineType,
      EmptyLineType,
      AnyLineType
    };

    LineInformation(Type Type = UnknownLineType, Address const& rAddr = Address())
      : m_Type(Type)
      , m_Address(rAddr)
    {}

    bool operator<(LineInformation const & li) const
    {
      if (m_Address < li.m_Address)       return true;
      else if (m_Address == li.m_Address) return m_Type > li.m_Type;
      else                                return false;
    }

    bool operator==(LineInformation const & li) const
    {
      return m_Type == li.m_Type && m_Address == li.m_Address;
    }

    Type GetType(void) const { return m_Type; }
    Address const& GetAddress(void) const { return m_Address; }

    void Update(LineInformation const& rLineInfo)
    {
      m_Type    = rLineInfo.m_Type;
      m_Address = rLineInfo.m_Address;
    }

  private:
    Type     m_Type;
    Address  m_Address;
  };

public:
  void AddLineInformation(LineInformation const & rLineInfo);
  void EraseLineInformation(LineInformation const & rLineInfo);
  void UpdateLineInformation(LineInformation const & rLineInfo);
  bool GetLineInformation(int Line, LineInformation & rLineInfo) const;
  bool ConvertLineInformationToLine(LineInformation const& rLineInfo, int & rLine) const;
  size_t GetNumberOfLine(void) const;
  void EraseAll(void);

private:
  typedef boost::mutex MutexType;
  typedef std::set<LineInformation> LineInformationContainer;

  size_t                   m_MaximumLineLength;
  mutable MutexType        m_EventMutex;
  LineInformationContainer m_Lines;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_VIEW_HPP__
