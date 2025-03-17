#include "moduleA.hpp"
#include "moduleB.hpp"

namespace CHAT::Module {
void ModuleA::init() {
    std::cout << "ModuleA::init()" << std::endl;
    ModuleB moduleB;
    std::cout << moduleB.name() << std::endl;
}

std::string ModuleA::name() const{
    return "ModuleA";
}
}
extern "C" CHAT::Utils::Module::ModuleBase* createModule() {
    return new CHAT::Module::ModuleA();
}
