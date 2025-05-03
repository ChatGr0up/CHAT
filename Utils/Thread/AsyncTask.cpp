#include "AsyncTask.hpp"
#include <chrono>

namespace CHAT::Utils::Thread {
AsyncTask::~AsyncTask()
{
    stopAsyncTask();
}

bool AsyncTask::startAsyncTask()
{
    if (m_isRunning) {
        return true;
    } 
    m_isRunning = true;
    m_loopInterval = loopInterval();
    try {
        auto self  = shared_from_this(); // 保证线程对象使用当前对象的时候当前对象还是存活
        m_thread = std::thread([self](){
            self->loop();
        });
        m_threadId = m_thread.get_id();
        return true;
    } catch (...) {
        m_isRunning = false;
        return false;
    }
}

bool AsyncTask::stopAsyncTask()
{
    m_isRunning = false;
    if (m_thread.joinable()) {
        m_thread.join();
        return true;
    }
    return false;
}

void AsyncTask::loop()
{
    while (m_isRunning) {
        if (!svc()) {
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(m_loopInterval));
    }
}

bool AsyncTask::svc()
{
    return true;
}

uint32_t AsyncTask::loopInterval()
{
    return 0;
}
}