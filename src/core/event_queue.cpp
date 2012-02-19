#include "medusa/event_queue.hpp"
#include <boost/thread/thread_time.hpp>

MEDUSA_NAMESPACE_BEGIN

//XXX: We should probably put a lock here, but it looks to deadlock the whole thing :(
void EventQueue::Push(EventHandler::EventType const& rEvent)
{
  m_Queue.push(rEvent);
  m_CondVar.notify_one();
}

void EventQueue::Quit(void)
{
  Push(EventHandler::Quit());
}

bool EventQueue::WaitAndHandleEvent(EventHandler& rEvtHdl)
{
  boost::unique_lock<MutexType> Lock(m_Mutex);

  while (m_Queue.empty())
    m_CondVar.wait(Lock);

  bool Result = rEvtHdl(m_Queue.front());
  m_Queue.pop();

  return Result;
}

void EventQueue::ProcessQueue(EventHandler& rEvtHdl)
{
  while (WaitAndHandleEvent(rEvtHdl)) {}
}

MEDUSA_NAMESPACE_END