#pragma once

#include <atomic>
#include <thread>
#include <memory>

namespace CHAT::Utils::Thread {
class AsyncTask : public std::enable_shared_from_this<AsyncTask> {
public:
    AsyncTask() = default;

    virtual ~AsyncTask(); 

    bool startAsyncTask();

    bool stopAsyncTask();

protected:
    virtual uint32_t loopInterval(); // 指定循环间隙
    
    virtual uint32_t loopDelayTime(); // 指定循环延迟

    virtual bool svc(); // 给出具体定时任务，返回true则永久循环
    
private:
    void loop();

protected:
    std::atomic<bool> m_isRunning{false};
    std::thread m_thread;
    std::thread::id m_threadId{};
    uint32_t m_loopInterval{0};
}; 
}