#include "ModuleLoader.hpp"
#include "EnvManager/EnvManager.hpp"
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
        std::cout << "Loading module: " << path << std::endl;
        const char* pathCStr = path.c_str();
        std::unique_ptr<void, decltype(&dlclose)> handle(dlopen(pathCStr, RTLD_LAZY), dlclose);
        if (!handle) {
            std::cerr << "Failed to load " << module << ": " << dlerror() << std::endl;
            continue;
        }

        using CreateModuleFunc = ModuleBase* (*)();
        auto createModule = reinterpret_cast<CreateModuleFunc>(dlsym(handle.get(), "createModule"));
        if (!createModule) {
            std::cerr << "No createModule function in " << module << std::endl;
            continue;
        }

        std::unique_ptr<ModuleBase, ModuleDeleter> moduleInstance(createModule());
        moduleInstance->init();  
        handles.emplace_back(std::move(handle));
        instances.emplace_back(std::move(moduleInstance));
    }
}    
}
