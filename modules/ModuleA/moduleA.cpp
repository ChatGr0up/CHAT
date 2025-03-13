#include "moduleA.hpp"

namespace CHAT::Module {
void ModuleA::init() {
    std::cout << "ModuleA::init()" << std::endl;
}

std::string ModuleA::name() const{
    return "ModuleA";
}
}