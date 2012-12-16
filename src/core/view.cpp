#include "medusa/view.hpp"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>

MEDUSA_NAMESPACE_USE;

void View::AddLineInformation(View::LineInformation const & rLineInfo)
{
  assert(rLineInfo.GetType() != LineInformation::AnyLineType);

  boost::recursive_mutex::scoped_lock(m_EventMutex);
  m_Set.insert(rLineInfo);
}

void View::EraseLineInformation(LineInformation const & rLineInfo)
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  auto itLineInfo = m_Set.find(rLineInfo);
  if (itLineInfo == std::end(m_Set)) return;
  m_Set.erase(itLineInfo);
}

void View::UpdateLineInformation(View::LineInformation const & rLineInfo)
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);

  EraseLineInformation(rLineInfo);
  AddLineInformation(rLineInfo);
}

bool View::GetLineInformation(int Line, View::LineInformation & rLineInfo) const
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  if (Line >= m_Set.size())
    return false;

  auto itLine = m_Set.begin();
  std::advance(itLine, Line);
  rLineInfo = *itLine;
  return true;
}

bool View::ConvertLineInformationToLine(View::LineInformation const& rLineInfo, int & rLine) const
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);

  auto itLineInfo = m_Set.find(rLineInfo);

  if (itLineInfo == std::end(m_Set) || rLineInfo < *itLineInfo) return false;

  rLine = static_cast<int>(std::distance(std::begin(m_Set), itLineInfo));
  return true;
}

size_t View::GetNumberOfLine(void) const
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  return m_Set.size();
}

void View::EraseAll(void)
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  m_Set.erase(std::begin(m_Set), std::end(m_Set));
}
