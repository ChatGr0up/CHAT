#pragma once

#include <string>

namespace CHAT::Utils::Module {
class ModuleBase {
public:
    ModuleBase() = default;
    virtual ~ModuleBase() = default;
    virtual void init() = 0;  
    virtual std::string name() const = 0;       
};
}
