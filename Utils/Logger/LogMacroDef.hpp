#pragma once

#include "Logger.hpp"

#define TRACE(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::TRACE, module, msg)
#define INFO(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::INFO, module, msg)
#define WARN(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::WARN, module, msg)
#define ERROR(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::ERROR, module, msg)
#define FATAL(module, msg) CHAT::Utils::Log::Logger::getInstance().log(LogLevel::FATAL, module, msg)
