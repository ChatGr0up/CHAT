#include "moduleB.hpp"

namespace CHAT::Module {
void ModuleB::init() {
    std::cout << "ModuleB::init()" << std::endl;
}

std::string ModuleB::name() const{
    return "ModuleB";
}
}
extern "C" CHAT::Utils::Module::ModuleBase* createModule() {
    return new CHAT::Module::ModuleB();
}