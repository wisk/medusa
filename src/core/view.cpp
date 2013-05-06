#include "medusa/view.hpp"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>

MEDUSA_NAMESPACE_USE;

void View::AddLineInformation(View::LineInformation const & rLineInfo)
{
  //assert(rLineInfo.GetType() != LineInformation::AnyLineType);

  //boost::recursive_mutex::scoped_lock(m_EventMutex);
  //m_Lines.insert(rLineInfo);
}

void View::EraseLineInformation(LineInformation const & rLineInfo)
{
  //boost::recursive_mutex::scoped_lock(m_EventMutex);
  //m_Lines.erase(rLineInfo);
}

void View::UpdateLineInformation(View::LineInformation const & rLineInfo)
{
  //boost::recursive_mutex::scoped_lock(m_EventMutex);

  //EraseLineInformation(rLineInfo);
  //AddLineInformation(rLineInfo);
}

bool View::GetLineInformation(int Line, View::LineInformation & rLineInfo) const
{
  //boost::recursive_mutex::scoped_lock(m_EventMutex);
  //if (Line >= m_Lines.size())
  //  return false;

  //auto itLine = m_Lines.begin();
  //std::advance(itLine, Line);
  //rLineInfo = *itLine;
  return true;
}

bool View::ConvertLineInformationToLine(View::LineInformation const& rLineInfo, int & rLine) const
{
  //boost::recursive_mutex::scoped_lock(m_EventMutex);

  //auto itLineInfo = m_Lines.find(rLineInfo);

  //if (itLineInfo == std::end(m_Lines) || rLineInfo < *itLineInfo)
  //  return false;

  //rLine = static_cast<int>(std::distance(std::begin(m_Lines), itLineInfo));
  return true;
}

size_t View::GetNumberOfLine(void) const
{
  //boost::recursive_mutex::scoped_lock(m_EventMutex);
  //return m_Lines.size();
  return 0;
}

void View::EraseAll(void)
{
  //boost::recursive_mutex::scoped_lock(m_EventMutex);
  //m_Lines.erase(std::begin(m_Lines), std::end(m_Lines));
}
