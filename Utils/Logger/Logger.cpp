#include "Logger.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "EnvManager/EnvManager.hpp"
#include "JsonUtils/JsonUtils.hpp"
#include "FileUtils/fileSystem.hpp"
namespace CHAT::Utils::Log {
Logger::Logger()
{
    initLogConfig();
}

void Logger::initLogConfig() 
{
    std::string configFilePath = CHAT::Utils::EnvManager::EnvManager::getInstance().getGlobalConfigPath() + 
        "/LoggerConfig.json";
    CHAT::Utils::Json::JsonUtils jsonUtils;
    CHAT::Utils::Json::JsonValue jsonValue = jsonUtils.loadFromFile(configFilePath);
    
    m_logDirectory = jsonValue.get("log_directory", "logs").asString();
    m_archiveDirectory = jsonValue.get("archive_directory", "logs/archives").asString();
    m_maxLogSize = jsonValue.get("log_max_size", 10485760).asInt();
    std::string logFilePath = m_logDirectory + "/log.trace";
    CHAT::Utils::FileUtils::fileSystem::createDirectory(logFilePath);
    // logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file.");
    }

    // 设置当前文件大小
    logFile.seekp(0, std::ios::end);
    currentLogSize = logFile.tellp();
}

// 获取当前时间的辅助函数
std::string Logger::getCurrentTime() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

const std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

// 写入日志文件（线程安全）
void Logger::writeLogToFile(const std::string& logMessage) {
    std::lock_guard<std::mutex> lock(logMutex);  // 自动加锁，确保线程安全

    if (currentLogSize >= maxLogSize) {
        archiveLogFile();
    }

    logFile << logMessage << '\n';
    logFile.flush();
    currentLogSize += logMessage.size();
}

// 文件归档（线程安全）
void Logger::archiveLogFile() {
    {
        std::lock_guard<std::mutex> lock(logMutex);
        if (isArchiving) return;
        isArchiving = true;
    }

    std::string archiveFilePath = archiveDirectory + "/log_" + getCurrentTime() + ".txt";
    {
        std::lock_guard<std::mutex> lock(logMutex);
        logFile.close();  // 关闭当前日志文件

        // 重命名并归档
        std::filesystem::rename(logDirectory + "/log.txt", archiveFilePath);

        // 重新打开日志文件
        logFile.open(logDirectory + "/log.txt", std::ios::out | std::ios::trunc);
        if (!logFile.is_open()) {
            isArchiving = false;
            throw std::runtime_error("Failed to reopen log file after archiving.");
        }
        currentLogSize = 0;  // 重置当前文件大小
        isArchiving = false;
    }
}

void Logger::log(LogLevel level, const std::string& moduleName, const std::string& message) {
    if (level < minLogLevel) return;

    std::string logMessage = "[" + getCurrentTime() + "] [" + logLevelToString(level) + "] [" + moduleName + "] " + message;
    writeLogToFile(logMessage);
}

}