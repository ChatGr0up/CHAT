#include "AsyncTask.hpp"
#include <thread>
#include <chrono>
#include <iostream>

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
        m_thread = std::thread([this](){
            std::this_thread::sleep_for(std::chrono::milliseconds(this->loopDelayTime()));
            this->loop();
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
    return 10;
}

uint32_t AsyncTask::loopDelayTime()
{
    return 0;
}
}