#include "ModuleBItf/moduleBItf.hpp"
#include "Module/ModuleBase.hpp"
#include <iostream>

namespace CHAT::Module {
class ModuleB : public CHAT::Module::ModuleBItf {
public:
    ModuleB() = default;
    ~ModuleB() = default;
    void init() override;
    std::string name() const override;
    void specialFunctionOfB() override;
};
}
