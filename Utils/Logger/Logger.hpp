#pragma once

#include "LoggerCommonDefine.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include "FileUtils/FileStream.hpp"
#include "Thread/AsyncTask.hpp"
#include <condition_variable>
namespace CHAT::Utils::Log {
class Logger : public Utils::Thread::AsyncTask {
public:
    static Logger& getInstance();

    void log(LogLevel level, const std::string& moduleName, const std::string& message);

protected:
    uint32_t loopInterval() override;
        
    uint32_t loopDelayTime() override;

    bool svc() override;

private:
    Logger();

    ~Logger();

    Logger(const Logger&) = delete;

    Logger& operator=(const Logger&) = delete;

    std::string getCurrentTime();

    void initLogConfigAndStart();

    static const std::string logLevelToString(LogLevel level);

    void writeLogToFile(std::vector<std::string> logs);

    void pushMessage(const std::string& message);

    void consumeCache(std::vector<std::string> &logs);

    void archiveThreadFunc();

    void requestArchive();

private:
    std::string m_logDirectory;
    std::string m_archiveDirectory;
    size_t m_maxLogSize;
    size_t m_currentLogSize;
    int m_flushCount{0};
    std::vector<std::string> m_logCache;
    std::mutex m_logMutex;
    bool m_isArchiving;
    std::string m_logFile;
    std::thread m_archivedThread;
    std::condition_variable m_conArchivedThread;
    std::mutex m_archivedThreadMutex;
    std::atomic<bool> m_archivedRequested;
    std::atomic<bool> m_exitArchivedThread;
    std::unique_ptr<Utils::FileUtils::FileStream<std::ofstream>> m_fileStream;
};
}
