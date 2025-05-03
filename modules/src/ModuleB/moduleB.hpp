#include "ModuleBItf/moduleBItf.hpp"
#include "Module/ModuleBase.hpp"
// #include "Logger/Logger.hpp"
#include <iostream>

namespace CHAT::Module {
// #define MODULEB_TRACE(msg) BASE_TRACE("CHAT::Module::ModuleB", msg);
class ModuleB : public CHAT::Module::ModuleBItf {
public:
    ModuleB() = default;
    ~ModuleB() = default;
    void init() override;
    std::string name() const override;
    void specialFunctionOfB() override;
};
}
