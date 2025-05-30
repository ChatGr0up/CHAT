#include "ModuleLoader.hpp"
#include "EnvManager/EnvManager.hpp"
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Utils::Module {
ModuleLoader& ModuleLoader::getInstance()
{
    static ModuleLoader instance;
    return instance;
}

void ModuleLoader::loadModules(const std::vector<std::string>& modules) {
    std::string libPath = CHAT::Utils::EnvManager::EnvManager::getInstance().getLibPath();
    for (const auto& module : modules) {
        std::string path = libPath + "/lib" + module + ".so";
        TRACE("CHAT::Utils::Module::ModuleLoader::loadModules", "Loading module: " + path);
        std::cout <<path<< std::endl;
        const char* pathCStr = path.c_str();
        std::unique_ptr<void, DlCloser> handle(dlopen(pathCStr, RTLD_LAZY), dlclose);
        if (!handle) {
            ERROR("CHAT::Utils::Module::ModuleLoader::loadModules", "Failed to load " + path + ": " + dlerror());
            continue;
        }
        using CreateModuleFunc = ModuleBase* (*)();
        auto createModule = reinterpret_cast<CreateModuleFunc>(dlsym(handle.get(), "createModule"));
        if (!createModule) {
            ERROR("CHAT::Utils::Module::ModuleLoader::loadModules", "No createModule function in " + path);
            continue;
        }
        std::unique_ptr<ModuleBase, ModuleDeleter> moduleInstance(createModule());
        moduleInstance->init();  
        handles.emplace_back(std::move(handle));
        instances.emplace_back(std::move(moduleInstance));
    }
}    
}
