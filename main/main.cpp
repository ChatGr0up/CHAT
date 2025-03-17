#include "Module/ModuleLoader.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <dlfcn.h>
#include "JsonUtils/JsonUtils.hpp"

int main() {
    CHAT::Utils::Module::ModuleLoader loader;
    CHAT::Utils::Json::JsonUtils jsonUtils;
    std::filesystem::path execPath = std::filesystem::current_path();
    std::string moduleConfigPath = (execPath / "../../configs/ModuleDefine.json").string();
    CHAT::Utils::Json::JsonValue jsonValue = jsonUtils.loadFromFile(moduleConfigPath);
    std::vector<std::string> modules;
    if (jsonValue.contains("modules") && jsonValue["modules"].is_array()) {
        for (const auto& module : jsonValue["modules"]) {
            if (module.is_string()) {
                modules.push_back(module.get<std::string>());
            } else {
                std::cerr << "Module name must be string" << std::endl;
            }
        }
    } else {
        std::cerr << "ModuleDefine.json must contains modules array" << std::endl;
    }
    loader.loadModules(modules);  // 需要加载的模块
    return 0;
}
