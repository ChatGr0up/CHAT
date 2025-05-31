# pragma once

#include "ModuleLoader.hpp"
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Utils::Module {
template<typename T>    
static T& getModuleRef()
{
    return CHAT::Utils::Module::ModuleLoader::getInstance().getModule<T>();
}
}