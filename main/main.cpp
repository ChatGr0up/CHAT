#include "Module/ModuleLoader.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <dlfcn.h>
#include <filesystem>
#include "JsonUtils/JsonUtils.hpp"
#include <thread>
#include <condition_variable>
#include <csignal>
#include <atomic>
#include "FileUtils/fileSystem.hpp"
#include "EnvManager/EnvManager.hpp"
#include "Logger/LogMacroDef.hpp"

std::condition_variable chat_root_cv;
std::mutex char_root_mtx;;
std::atomic<bool> exitFlag = false;

void signalHandler(int signum) {
    std::cout << "Received signal " << signum << ", shutting down..." << std::endl;
    exitFlag = true;
    chat_root_cv.notify_all();  // 解除等待，让 main 退出
}

int main() {
    std::cout << "Starting CHAT service..." << std::endl;
    TRACE("CHAT::MAIN", "initializing the full process!");
    CHAT::Utils::Json::JsonUtils jsonUtils;
    std::string configRootPath = CHAT::Utils::EnvManager::EnvManager::getInstance().getGlobalConfigPath();
    std::string moduleConfigPath = configRootPath + "/ModuleDefine.json";
    CHAT::Utils::Json::JsonValue jsonValue = jsonUtils.loadFromFile(moduleConfigPath);
    std::vector<std::string> modules;
    if (jsonValue.isMember("modules") && jsonValue["modules"].isArray()) {
        for (const auto& module : jsonValue["modules"]) {
            if (module.isString()) {
                modules.push_back(module.asString());
            } else {
                ERROR("CHAT::MAIN", "Module name must be string");
            }
        }
    } else {
        ERROR("CHAT::MAIN", "ModuleDefine.json must contains modules array");
    }
    CHAT::Utils::Module::ModuleLoader::getInstance().loadModules(modules);
    TRACE("CHAT::MAIN", "All modules initialized. Waiting for termination signal...");
    std::unique_lock<std::mutex> lk(char_root_mtx);
    signal(SIGINT, signalHandler);
    chat_root_cv.wait(lk, []{ return exitFlag.load(); });  
    TRACE("CHAT::MAIN", "Service stopping, cleaning up resources...");
    return 0;
}
