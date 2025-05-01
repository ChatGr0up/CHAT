#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <memory>

namespace CHAT::Utils::Module {
class ModuleBase {
public:
    ModuleBase() = default;
    virtual ~ModuleBase() = default;
    virtual void init() = 0;  
    virtual std::string name() const = 0;       
};

class ModuleRegsitry {
public:
    ModuleRegsitry(const ModuleRegsitry&) = delete;
    
    ModuleRegsitry(const ModuleRegsitry&&) = delete;

    ~ModuleRegsitry() = default;

    static ModuleRegsitry& getInstance() {
        static ModuleRegsitry instance;
        return instance;
    }

    template<typename T>
    void registerModule(std::shared_ptr<T> module) {
        const std::type_index key(typeid(T));
        modulesCache[key] = module;
    }

    template<typename T>
    std::shared_ptr<T> getModuleRef() {
        const std::type_index key(typeid(T));
        auto it = modulesCache.find(key);
        if (it == modulesCache.end()) {
            return nullptr;
        }
        return std::static_pointer_cast<T>(modulesCache[key]);
    }
 
private:
    ModuleRegsitry() = default;

private:
    using ModulesCache = std::unordered_map<std::type_index, std::shared_ptr<ModuleBase>>;
    ModulesCache modulesCache;
};
}
