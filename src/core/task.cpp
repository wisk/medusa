#include "medusa/task.hpp"

MEDUSA_NAMESPACE_BEGIN

TaskManager::TaskManager(NotifyFunctionType const& rNotify)
: m_Running(false)
, m_Notify(rNotify)
{
  Start();
}

TaskManager::~TaskManager(void)
{
  if (m_Running)
    Stop();
}

void TaskManager::Start(void)
{
  if (m_Running)
    return;

  m_Running = true;
  m_Thread = std::thread([&]()
  {
    Task* pCurTask = nullptr;

    while (true)
    {
      { std::unique_lock<std::mutex> Lock(m_Mutex);

        if (!m_Running && m_Tasks.empty())
          break;

        while (m_Tasks.empty())
          m_CondVar.wait(Lock);

        pCurTask = m_Tasks.front();
      }

      if (pCurTask == nullptr)
      {
        m_Running = false;
        break;
      }
      pCurTask->Run();

      { std::unique_lock<std::mutex> Lock(m_Mutex);
        m_Tasks.pop();
      }
      m_Notify(pCurTask);
      delete pCurTask;
    }
  });
}

void TaskManager::Stop(void)
{
  if (!m_Running)
    return;

  m_Running = false;

  m_Mutex.lock();
  m_Tasks.push(nullptr);
  m_CondVar.notify_one();
  m_Mutex.unlock();
  if (m_Thread.joinable())
    m_Thread.join();
}

void TaskManager::Wait(void)
{
  { std::unique_lock<std::mutex> Lock(m_Mutex);
    if (m_Tasks.empty())
      return;
  }
  m_Running = false;
  if (m_Thread.joinable())
    m_Thread.join();
}

bool TaskManager::AddTask(Task* pTask)
{
  if (!m_Running)
    Start();

  if (pTask == nullptr)
    return false;

  { std::unique_lock<std::mutex> Lock(m_Mutex);
    m_Tasks.push(pTask);
  }
  m_CondVar.notify_one();
  return true;
}

MEDUSA_NAMESPACE_END
