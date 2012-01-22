#ifndef _MEDUSA_EVENT_QUEUE_
#define _MEDUSA_EVENT_QUEUE_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/event_handler.hpp"

#include <queue>
#include <boost/thread.hpp>

MEDUSA_NAMESPACE_BEGIN

class EventQueue : public boost::noncopyable
{
public:
  typedef boost::mutex MutexType;

  void Push(EventHandler::EventType const& rEvent);
  void Quit(void);
  bool WaitAndHandleEvent(EventHandler& rEvtHdl);

  void ProcessQueue(EventHandler& rEvtHdl);

private:
  std::queue<EventHandler::EventType> m_Queue;
  boost::condition_variable           m_CondVar;
  mutable boost::mutex                m_Mutex;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EVENT_QUEUE_