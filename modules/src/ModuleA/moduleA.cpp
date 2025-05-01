#include "moduleA.hpp"

namespace CHAT::Module {
void ModuleA::init() {
    std::cout<< "called :: ModuleA::init" << std::endl;
}

std::string ModuleA::name() const{
    return "ModuleA";
}

void ModuleA::processResult(MYSQL_RES* result)
{
    return;
}

}
extern "C" CHAT::Module::ModuleAItf* createModule() {
    return new CHAT::Module::ModuleA();
}
