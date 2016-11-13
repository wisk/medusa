#include <chrono>
#include <sstream>
#include <iomanip>

#include "medusa/task.hpp"
#include "medusa/log.hpp"

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

      auto Beg = std::chrono::system_clock::now();
      pCurTask->Run();
      auto End = std::chrono::system_clock::now();

      auto hr = std::chrono::duration_cast<std::chrono::hours>       (End - Beg).count();
      auto mn = std::chrono::duration_cast<std::chrono::minutes>     (End - Beg).count() % 60;
      auto sc = std::chrono::duration_cast<std::chrono::seconds>     (End - Beg).count() % 60;
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(End - Beg).count() % 1000;

      std::ostringstream Time;
      Time << std::setfill('0')
        << std::setw(2) << hr
        << ":"
        << std::setw(2) << mn 
        << ":"
        << std::setw(2) << sc
        << ":"
        << std::setw(3) << ms;

      Log::Write("core") << "task \"" << pCurTask->GetName() << "\" finished in " << Time.str() << LogEnd;

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
