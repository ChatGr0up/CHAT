#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <memory>
#include <mutex>
#include <thread>

namespace CHAT::Utils::Log {
enum class LogLevel {
    TRACE,
    INFO,
    WARN,
    ERROR,
    FATAL
};

#define BASE_TRACE(module, msg) Logger::log(LogLevel::TRACE, module, msg)
#define BASE_INFO(module, msg) Logger::log(LogLevel::INFO, module, msg)
#define BASE_WARN(module, msg) Logger::log(LogLevel::WARN, module, msg)
#define BASE_ERROR(module, msg) Logger::log(LogLevel::ERROR, module, msg)
#define BASE_FATAL(module, msg) Logger::log(LogLevel::FATAL, module, msg)

// 获取当前时间的辅助函数
std::string getCurrentTime();

// 基础日志类
class Logger {
public:
    static void log(LogLevel level, const std::string& moduleName, const std::string& message);

    // 设置日志的最小级别（只记录该级别及其以上的日志）
    static LogLevel minLogLevel;

    // 配置文件的日志路径和大小
    static void initLogConfig(const std::string& configFilePath);

private:
    static const std::string logLevelToString(LogLevel level);
    static void writeLogToFile(const std::string& logMessage);
    static void archiveLogFile();

    static std::string logDirectory;
    static std::string archiveDirectory;
    static size_t maxLogSize;
    static std::ofstream logFile;
    static size_t currentLogSize;
    static std::mutex logMutex;  // 互斥锁，用于确保日志写入的线程安全
};
}
