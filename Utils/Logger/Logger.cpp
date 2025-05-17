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
#include "FileUtils/FileStream.hpp"
#include "TimeUtils/TimeUtils.hpp"
#include <sstream>
#include <unistd.h>
#include <sys/syscall.h>
namespace CHAT::Utils::Log {
Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{
    initLogConfigAndStart();
}

Logger::~Logger()
{
    if (m_archivedThread.joinable()) {
        {
            std::lock_guard<std::mutex> lock(m_archivedThreadMutex);
            m_exitArchivedThread = true;
        }
        m_conArchivedThread.notify_one();
        m_archivedThread.join();
    }
}

void Logger::initLogConfigAndStart() 
{
    std::string configFilePath = CHAT::Utils::EnvManager::EnvManager::getInstance().getGlobalConfigPath() + 
        "/LoggerConfig.json";
    CHAT::Utils::Json::JsonUtils jsonUtils;
    CHAT::Utils::Json::JsonValue jsonValue = jsonUtils.loadFromFile(configFilePath);
    
    std::string logRootPath = Utils::EnvManager::EnvManager::getInstance().getLogPath();
    m_logDirectory = logRootPath + "/" + jsonValue.get("log_directory", "logs").asString();
    m_archiveDirectory = logRootPath + "/" + jsonValue.get("archive_directory", "logs/archives").asString();
    m_maxLogSize = jsonValue.get("log_max_size", 100 * 1024 * 1024).asInt();
    m_flushCount = jsonValue.get("flush_account", 1000000).asInt();
    m_currentLogSize = 0;
    m_logFile =  m_logDirectory + "/log.trace";
    CHAT::Utils::FileUtils::fileSystem::createDirectory(m_logDirectory);
    CHAT::Utils::FileUtils::fileSystem::createDirectory(m_archiveDirectory);
    m_fileStream = std::make_unique<Utils::FileUtils::FileStream<std::ofstream>>(m_logFile, std::ios::out | std::ios::app);
    if (!m_fileStream->isGood()) {
        throw std::runtime_error("Failed to open log file.");
    }
    std::cout << m_logFile << std::endl;
    m_archivedThread = std::thread(&Logger::archiveThreadFunc, this);
    startAsyncTask();
}

std::string Logger::getCurrentTime() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S");
    oss << "-" << std::setfill('0') << std::setw(3) << ms.count();
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

void Logger::writeLogToFile(std::vector<std::string> logs)
{
    std::ostringstream oss;
    size_t newDataSize = 0;
    for (const auto& logMessage : logs) {
        oss << logMessage << '\n';
        newDataSize += logMessage.size() + 1;
    }
    (*m_fileStream) << oss.str();
    if (!m_fileStream->isGood()) {
        std::cerr << "Warning: Failed to write log to file." << std::endl;
    }
    m_fileStream->flush();

    m_currentLogSize += 1;
    if (m_currentLogSize >= m_flushCount || isFileTooBig(m_logFile)) {
        requestArchive();
    }
}



void Logger::log(LogLevel level, const std::string& moduleName, const std::string& message) 
{
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);
    std::string logMessage = "[" + getCurrentTime() + "] [PID " + std::to_string(pid) + "] [TID " + std::to_string(tid) +
         "] [" + logLevelToString(level) + "] [" + moduleName + "] " + message;
    pushMessage(logMessage);
}

void Logger::pushMessage(const std::string& message)
{
    std::lock_guard lockGuard(m_logMutex);
    m_logCache.emplace_back(message);
}

uint32_t Logger::loopInterval()
{
    return 1;
}

bool Logger::svc()
{
    std::vector<std::string> tempLogs;
    consumeCache(tempLogs);
    writeLogToFile(std::move(tempLogs));
    return true;
}

void Logger::consumeCache(std::vector<std::string>& logs)
{
    std::lock_guard lockGuard(m_logMutex);
    logs.swap(m_logCache);
}

void Logger::archiveThreadFunc() {
    while (true) {
        std::unique_lock<std::mutex> lock(m_archivedThreadMutex);
        m_conArchivedThread.wait(lock, [this] { return m_archivedRequested || m_exitArchivedThread; });

        if (m_exitArchivedThread) break;
        m_archivedRequested = false;
        lock.unlock();
        {
            std::lock_guard<std::mutex> logLock(m_logMutex);
            std::string archivePath = m_archiveDirectory + "/" + getCurrentTime() + "-" + std::to_string(++m_logArchivedCount) + "_log.txt";
            std::cout << archivePath << std::endl;
            m_fileStream->flush();
            m_fileStream->close();
            CHAT::Utils::FileUtils::fileSystem::renameFile(m_logFile, archivePath);
            try {
                m_fileStream = std::make_unique<Utils::FileUtils::FileStream<std::ofstream>>(m_logFile, std::ios::out | std::ios::trunc);
            } catch (const std::exception& e) {
                std::cerr << "Log archive reopen failed: " << e.what() << std::endl;
            }
            m_currentLogSize = 0;
        }
    }
}

void Logger::requestArchive()
{
    {
        std::lock_guard<std::mutex> lock(m_archivedThreadMutex);
        m_archivedRequested = true;
    }
    m_conArchivedThread.notify_one();
}

bool Logger::isFileTooBig(const std::string &fileName)
{
    Utils::FileUtils::FileStat fileStat;
    if (!Utils::FileUtils::fileSystem::getFileStat(fileName, fileStat)) {
        std::cout << "fatal error!, cannot getFileStat!" << std::endl;
        return false;
    }
    if (fileStat.st_size >= m_maxLogSize) {
        return true;
    }
    return false;
}
}