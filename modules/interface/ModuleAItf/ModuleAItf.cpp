#include "ModuleAItf/ModuleAItf.hpp"

namespace CHAT::Module {
std::string ModuleAItf::name() const
{
    const std::string moduleName("ModuleA");
    return moduleName;
}
}