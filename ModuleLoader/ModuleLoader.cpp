#include "ModuleLoader.hpp"
#include "Utils/FileUtils/fileSystem.hpp"

namespace CHAT::ModuleLoader {
void ModuleLoader::load() {
    // std::string configPath = Utils::FileUtils::fileSystem::currentPath() + "ModuleDefine.json";
    // if (!Utils::FileUtils::fileSystem::fileExists(configPath)) {
    //     // todo : add log
    //     return;
    // }
    // Utils::JsonUtils::json jsonObject = Utils::JsonUtils::json::parse(Utils::FileUtils::fileSystem::readFile(configPath));
    // for (auto& module : jsonObject["modules"]) {
    //     std::string moduleName = module["name"];
    //     std::string moduleType = module["type"];
    //     if (moduleType == "dll") {
    //         // todo : load dll
    //     } else if (moduleType == "so") {
    //         // todo : load so
    //     }
    // }
    
}
}