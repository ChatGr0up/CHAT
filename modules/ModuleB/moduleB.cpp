#include "moduleB.hpp"

namespace CHAT::Module {
void ModuleB::init() {
    std::cout << "ModuleB::init()" << std::endl;
}

std::string ModuleB::name() const{
    return "ModuleB";
}
}