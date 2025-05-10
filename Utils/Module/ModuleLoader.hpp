#include "ModuleBase.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <dlfcn.h>
#include <unordered_map>
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
    std::shared_ptr<T> getModule()
    {
        // make sure the object is exist if you want to call this function
        for (const auto& module : instances) {
            if (auto ptr = dynamic_cast<T*>(module.get())) {
                return std::shared_ptr<T>(ptr, [](T*){});
            }
        }
        return nullptr;
    }

private:
    ModuleLoader() = default;    

private:
    std::vector<std::unique_ptr<void, DlCloser>> handles; 
    std::vector<std::unique_ptr<ModuleBase, ModuleDeleter>> instances; 
    std::unordered_map<std::string, ModuleBase*> instanceMap;
};
}