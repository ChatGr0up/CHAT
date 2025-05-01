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

std::condition_variable chat_root_cv;
std::mutex char_root_mtx;;
std::atomic<bool> exitFlag = false;

void signalHandler(int signum) {
    std::cout << "Received signal " << signum << ", shutting down..." << std::endl;
    exitFlag = true;
    chat_root_cv.notify_all();  // 解除等待，让 main 退出
}

int main() {
    std::cout << "initializing the full process!" << std::endl;
    CHAT::Utils::Json::JsonUtils jsonUtils;
    std::string moduleConfigPath = ("../../configs/ModuleDefine.json");
    CHAT::Utils::Json::JsonValue jsonValue = jsonUtils.loadFromFile(moduleConfigPath);
    std::vector<std::string> modules;
    if (jsonValue.isMember("modules") && jsonValue["modules"].isArray()) {
        for (const auto& module : jsonValue["modules"]) {
            if (module.isString()) {
                modules.push_back(module.asString());
            } else {
                std::cerr << "Module name must be string" << std::endl;
            }
        }
    } else {
        std::cerr << "ModuleDefine.json must contains modules array" << std::endl;
    }
    CHAT::Utils::Module::ModuleLoader::getInstance().loadModules(modules);
    std::cout << "All modules initialized. Waiting for termination signal..." << std::endl;
    std::unique_lock<std::mutex> lk(char_root_mtx);
    signal(SIGINT, signalHandler);
    chat_root_cv.wait(lk, []{ return exitFlag.load(); });  
    std::cout << "Service stopping, cleaning up resources..." << std::endl;
    return 0;
}
