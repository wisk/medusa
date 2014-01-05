#ifndef __MEDUSA_TASK_HPP__
#define __MEDUSA_TASK_HPP__

#include "medusa/namespace.hpp"

#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <functional>

MEDUSA_NAMESPACE_BEGIN

class Task
{
public:
  virtual ~Task(void) {}
  virtual std::string GetName(void) const = 0;
  virtual void Run(void) = 0;
};

class TaskManager
{
public:
  typedef std::function<void (Task const*)> NotifyFunctionType;

  TaskManager(NotifyFunctionType const& rNotify);
  ~TaskManager(void);

  void Start(void);
  void Stop(void);
  void Wait(void);

  void AddTask(Task* pTask);

private:
  std::atomic<bool>       m_Running;
  std::thread             m_Thread;
  std::condition_variable m_CondVar;
  std::queue<Task*>       m_Tasks;
  std::mutex              m_Mutex;
  NotifyFunctionType      m_Notify;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_TASK_HPP__
