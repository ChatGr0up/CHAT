#include "ModuleBase.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <dlfcn.h>
#include <unordered_map>
#include "Logger/LogMacroDef.hpp"
namespace CHAT::Utils::Module {
using DlCloser = int (*)(void*);    
struct ModuleDeleter {
    void operator()(ModuleBase* module) const {
        delete module;
    }
};
class ModuleLoader {
public:
    ModuleLoader(const ModuleLoader&) = default;

    ~ModuleLoader() = default;

    static ModuleLoader& getInstance();

    void loadModules(const std::vector<std::string>& modules);

    template<typename T>
    T& getModule()
    {
        for (const auto& module : instances) {
            if (auto ptr = dynamic_cast<T*>(module.get())) {
                return *ptr;
            }
        }
        ERROR("getModule", "failed to find module");
        throw std::runtime_error("module not found");
    }

private:
    ModuleLoader() = default;    

private:
    std::vector<std::unique_ptr<void, DlCloser>> handles; 
    std::vector<std::unique_ptr<ModuleBase, ModuleDeleter>> instances; 
    std::unordered_map<std::string, ModuleBase*> instanceMap;
};
}