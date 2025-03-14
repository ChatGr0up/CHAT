#include "Module/ModuleBase.hpp"
#include <iostream>

namespace CHAT::Module {
class ModuleA : public CHAT::Utils::Module::ModuleBase {
public:
    ModuleA() = default;
    ~ModuleA() = default;
    void init() override;
    std::string name() const override;
};
}
