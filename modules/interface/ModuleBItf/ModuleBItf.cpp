#include "ModuleBItf/moduleBItf.hpp"

namespace CHAT::Module {
std::string ModuleBItf::name() const
{
    const std::string moduleName("ModuleB");
    return moduleName;
}
}