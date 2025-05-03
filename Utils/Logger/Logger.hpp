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

#define BASE_TRACE(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::TRACE, module, msg)
#define BASE_INFO(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::INFO, module, msg)
#define BASE_WARN(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::WARN, module, msg)
#define BASE_ERROR(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::ERROR, module, msg)
#define BASE_FATAL(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::FATAL, module, msg)

namespace CHAT::Utils::Log {
class Logger {
public:
    static Logger& getInstance();

    void log(LogLevel level, const std::string& moduleName, const std::string& message);

private:
    Logger();

    Logger(const Logger&) = delete;

    Logger& operator=(const Logger&) = delete;

    std::string getCurrentTime();

    void initLogConfig(const std::string& configFilePath);

    static const std::string logLevelToString(LogLevel level);

    void writeLogToFile(const std::string& logMessage);

    void archiveLogFile();

private:
    std::string m_logDirectory;
    std::string m_archiveDirectory;
    size_t m_maxLogSize;
    std::ofstream m_logFile;
    size_t m_currentLogSize;
    std::vector<std::string> m_logCache;
    std::mutex m_logMutex;
    LogLevel m_minLogLevel;
    bool m_isArchiving;
};
}
