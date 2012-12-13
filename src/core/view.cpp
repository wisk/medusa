#include "medusa/view.hpp"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>

MEDUSA_NAMESPACE_USE;

void View::AddLineInformation(View::LineInformation const & rLineInfo)
{
  if (rLineInfo.GetType() == LineInformation::AnyLineType) return;

  boost::recursive_mutex::scoped_lock(m_EventMutex);
  auto itPrevLineInfo = std::lower_bound(std::begin(m_LinesInformation), std::end(m_LinesInformation), rLineInfo);
  if (itPrevLineInfo == m_LinesInformation.end() || rLineInfo < *itPrevLineInfo)
    m_LinesInformation.insert(itPrevLineInfo, rLineInfo);
}

void View::EraseLineInformation(LineInformation const & rLineInfo)
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  m_LinesInformation.erase(std::remove_if(std::begin(m_LinesInformation), std::end(m_LinesInformation), [&rLineInfo](LineInformation& const rCurLineInfo)
  {
    return ((rCurLineInfo.GetType() == rLineInfo.GetType()   ||
      //rCurLineInfo.GetType() == LineInformation::AnyLineType ||
      rCurLineInfo.GetAddress() == rLineInfo.GetAddress()));
  }));

  //for (auto itLineInfo = std::begin(m_LinesInformation); itLineInfo != std::end(m_LinesInformation); ++itLineInfo)
  //{
  //  if ((itLineInfo->GetType() == rLineInfo.GetType() || itLineInfo->GetType() == LineInformation::AnyLineType || itLineInfo->GetAddress() == rLineInfo.GetAddress()))
  //  {
  //    m_LinesInformation.erase(itLineInfo);
  //    return;
  //  }
  //}
}

void View::UpdateLineInformation(View::LineInformation const & rLineInfo)
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  auto itLineInfo = std::lower_bound(std::begin(m_LinesInformation), std::end(m_LinesInformation), rLineInfo);

  if (itLineInfo == std::end(m_LinesInformation) || rLineInfo < *itLineInfo)
  {
    AddLineInformation(rLineInfo);
    return;
  }

  *itLineInfo = rLineInfo;
}

bool View::GetLineInformation(int Line, View::LineInformation & rLineInfo) const
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  if (Line >= m_LinesInformation.size())
    return false;

  rLineInfo = m_LinesInformation[Line];
  return true;
}

bool View::ConvertLineInformationToLine(View::LineInformation const& rLineInfo, int & rLine) const
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  auto itLineInfo = std::lower_bound(std::begin(m_LinesInformation), std::end(m_LinesInformation), rLineInfo);

  if (itLineInfo == std::end(m_LinesInformation) || rLineInfo < *itLineInfo) return false;

  rLine = static_cast<int>(std::distance(std::begin(m_LinesInformation), itLineInfo));
  return true;
}

size_t View::GetNumberOfLine(void) const
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  return m_LinesInformation.size();
}

void View::EraseAll(void)
{
  boost::recursive_mutex::scoped_lock(m_EventMutex);
  m_LinesInformation.erase(std::begin(m_LinesInformation), std::end(m_LinesInformation));
}