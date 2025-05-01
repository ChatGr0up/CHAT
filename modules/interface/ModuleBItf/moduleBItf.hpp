#include "Module/ModuleBase.hpp"
#include <iostream>

namespace CHAT::Module {
class ModuleBItf : public CHAT::Utils::Module::ModuleBase {
public:
    ModuleBItf() = default;
    ~ModuleBItf() = default;
    std::string name() const override;
    virtual void specialFunctionOfB() = 0;
};
}
