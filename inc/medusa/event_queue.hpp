#ifndef MEDUSA_EVENT_QUEUE_HPP
#define MEDUSA_EVENT_QUEUE_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/event_handler.hpp"

#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

MEDUSA_NAMESPACE_BEGIN

class EventQueue : public boost::noncopyable
{
public:
  typedef std::mutex MutexType;

  void Push(EventHandler::EventType const& rEvent);
  void Quit(void);
  bool WaitAndHandleEvent(EventHandler& rEvtHdl);

  void ProcessQueue(EventHandler& rEvtHdl);

private:
  std::queue<EventHandler::EventType> m_Queue;
  std::condition_variable m_CondVar;
  mutable std::mutex m_Mutex;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EVENT_QUEUE_HPP