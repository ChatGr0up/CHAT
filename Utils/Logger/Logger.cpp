#include "Logger.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

namespace CHAT::Utils::Log {

// 初始化日志配置
std::string Logger::logDirectory = "logs";
std::string Logger::archiveDirectory = "archives";
size_t Logger::maxLogSize = 10485760;  // 默认10MB
std::ofstream Logger::logFile;
size_t Logger::currentLogSize = 0;
std::mutex Logger::logMutex;  // 初始化互斥锁

// 初始化配置文件
void Logger::initLogConfig(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open log configuration file.");
    }

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.find("log_directory=") == 0) {
            logDirectory = line.substr(14);
        } else if (line.find("log_max_size=") == 0) {
            maxLogSize = std::stoul(line.substr(13));
        } else if (line.find("archive_directory=") == 0) {
            archiveDirectory = line.substr(18);
        }
    }

    // 创建日志目录和归档目录
    std::filesystem::create_directories(logDirectory);
    std::filesystem::create_directories(archiveDirectory);

    // 打开日志文件
    std::string logFilePath = logDirectory + "/log.txt";
    logFile.open(logFilePath, std::ios::app);
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
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
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

    logFile << logMessage << std::endl;
    currentLogSize += logMessage.size();
}

// 文件归档（线程安全）
void Logger::archiveLogFile() {
    std::lock_guard<std::mutex> lock(logMutex);

    std::string archiveFilePath = archiveDirectory + "/log_" + getCurrentTime() + ".txt";
    logFile.close();  // 关闭当前日志文件

    // 重命名并归档
    std::filesystem::rename(logDirectory + "/log.txt", archiveFilePath);

    // 重新打开日志文件
    logFile.open(logDirectory + "/log.txt", std::ios::out | std::ios::trunc);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to reopen log file after archiving.");
    }
    currentLogSize = 0;  // 重置当前文件大小
}

void Logger::log(LogLevel level, const std::string& moduleName, const std::string& message) {
    if (level < minLogLevel) return;

    std::string logMessage = "[" + getCurrentTime() + "] [" + logLevelToString(level) + "] [" + moduleName + "] " + message;
    writeLogToFile(logMessage);
}

}