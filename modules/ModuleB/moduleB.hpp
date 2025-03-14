#include "Module/ModuleBase.hpp"
#include <iostream>

namespace CHAT::Module {
class ModuleB : public CHAT::Utils::Module::ModuleBase {
public:
    ModuleB() = default;
    ~ModuleB() = default;
    void init() override;
    std::string name() const override;
};
}
