#include "EnvManager.hpp"
#include <stdio.h>
#include <iostream>

namespace CHAT::Utils::EnvManager {
EnvManager& EnvManager::getInstance()
{
    static EnvManager instance;
    return instance;
}

std::string EnvManager::getEnvByStr(const std::string& env)
{
    std::string envStr(std::getenv(env.c_str()));
    return envStr;
}

std::string EnvManager::getGlobalConfigPath()
{
    std::string envStr(std::getenv("GLOBAL_CONFIG_PATH"));
    if (envStr.empty()) {
        std::cerr << "fatal error!" << std::endl;
    }
    return envStr;
}
std::string EnvManager::getLibPath()
{
    std::string envStr(std::getenv("LIB_PATH"));
    if (envStr.empty()) {
        std::cerr << "fatal error!" << std::endl;
    }
    return envStr;
}
std::string EnvManager::getLogPath()
{
    std::string envStr(std::getenv("LOG_DIR"));
    if (envStr.empty()) {
        std::cerr << "fatal error!" << std::endl;
    }
    return envStr;
}
std::string EnvManager::getRestServiceConfigPath()
{
    std::string envStr(std::getenv("GLOBAL_REST_CONFIG_PATH"));
    if (envStr.empty()) {
        std::cerr << "fatal error!" << std::endl;
    }
    return envStr;
}
}