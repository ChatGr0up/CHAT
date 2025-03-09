#pragma once

#include "ModuleBase/ModuleBase.hpp"

namespace CHAT::ModuleLoader {
class ModuleLoader : public Module::ModuleBase {
public:
    ModuleLoader() = default;
    ~ModuleLoader() = default;
    ModuleLoader(const ModuleLoader&) = delete;
    ModuleLoader& operator=(const ModuleLoader&) = delete;
    void load() override;
    std::string moduleName() override;
private:
    std::string moduleName;      
};
} // namespace CHAT::ModuleLoader