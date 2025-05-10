#pragma once

#include "Logger.hpp"

#define TRACE(module, msg) CHAT::Utils::Log::Logger::getInstance().log(CHAT::Utils::Log::LogLevel::TRACE, module, msg)
#define INFO(module, msg) CHAT::Utils::Log::Logger::getInstance().log(CHAT::Utils::Log::LogLevel::INFO, module, msg)
#define WARN(module, msg) CHAT::Utils::Log::Logger::getInstance().log(CHAT::Utils::Log::LogLevel::WARN, module, msg)
#define ERROR(module, msg) CHAT::Utils::Log::Logger::getInstance().log(CHAT::Utils::Log::LogLevel::ERROR, module, msg)
#define FATAL(module, msg) CHAT::Utils::Log::Logger::getInstance().log(CHAT::Utils::Log::LogLevel::FATAL, module, msg)
