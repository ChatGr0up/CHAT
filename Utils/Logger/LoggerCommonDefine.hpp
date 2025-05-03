#pragma once

namespace CHAT::Utils::Log {
enum class LogLevel {
    TRACE,
    INFO,
    WARN,
    ERROR,
    FATAL
};

const int MAX_LOG_NUM = 500;
}