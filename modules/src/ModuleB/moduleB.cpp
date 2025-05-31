#include "moduleB.hpp"
#include "ModuleAItf/ModuleAItf.hpp"
#include "Module/ModuleUtils.hpp"

namespace CHAT::Module {
void ModuleB::init() {
    std::cout << "ModuleB::init()" << std::endl;
    CHAT::Module::ModuleAItf& moduleA = CHAT::Utils::Module::getModuleRef<CHAT::Module::ModuleAItf>();
    moduleA.init();
    // MODULEB_TRACE("test the logger!");
}

std::string ModuleB::name() const{
    return "ModuleB";
}

void ModuleB::specialFunctionOfB() {
    return ;
}
}
extern "C" CHAT::Module::ModuleBItf* createModule() {
    return new CHAT::Module::ModuleB();
}
