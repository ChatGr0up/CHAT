#include "ModuleBase.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <dlfcn.h>
namespace CHAT::Utils::Module {
struct ModuleDeleter {
    void operator()(ModuleBase* module) const {
        delete module;
    }
};
class ModuleLoader {
public:
    ModuleLoader() = default;
    ~ModuleLoader() = default;
    void loadModules(const std::vector<std::string>& modules);

private:
    std::vector<std::unique_ptr<void, decltype(&dlclose)>> handles; 
    std::vector<std::unique_ptr<ModuleBase, ModuleDeleter>> instances; 
};
}