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

    while (m_Running)
    {
      { std::unique_lock<std::mutex> Lock(m_Mutex);

        while (m_Tasks.empty())
          m_CondVar.wait(Lock);

        pCurTask = m_Tasks.front();
        m_Tasks.pop();

      }

      if (pCurTask == nullptr)
      {
        m_Running = false;
        break;
      }
      pCurTask->Run();
      m_Notify(pCurTask);
      delete pCurTask;
    }

    while (!m_Tasks.empty())
    {
      { std::unique_lock<std::mutex> Lock(m_Mutex);
      auto pCurTask = m_Tasks.front();
      m_Tasks.pop();
      }

      if (pCurTask)
      {
        pCurTask->Run();
        delete pCurTask;
      }
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
  m_Thread.join();
}

void TaskManager::AddTask(Task* pTask)
{
  if (!m_Running)
  {
    delete pTask;
    return;
  }

  if (pTask == nullptr)
    return;

  std::unique_lock<std::mutex> Lock(m_Mutex);
  m_Tasks.push(pTask);
  m_CondVar.notify_one();
}

MEDUSA_NAMESPACE_END
