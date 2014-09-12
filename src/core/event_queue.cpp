#include "medusa/event_queue.hpp"

MEDUSA_NAMESPACE_BEGIN

void EventQueue::Push(EventHandler::EventType const& rEvent)
{
  std::unique_lock<MutexType> Lock(m_Mutex);

  m_Queue.push(rEvent);
  Lock.unlock();
  m_CondVar.notify_one();
}

void EventQueue::Quit(void)
{
  Push(EventHandler::Quit());
}

bool EventQueue::WaitAndHandleEvent(EventHandler& rEvtHdl)
{
  std::unique_lock<MutexType> Lock(m_Mutex);

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