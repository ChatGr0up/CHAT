# pragma once

#include "ModuleLoader.hpp"

namespace CHAT::Utils::Module {
template<typename T>    
static T& getModuleRef()
{
    std::shared_ptr<T> ptr = CHAT::Utils::Module::ModuleLoader::getInstance().getModule<T>();
    if (ptr == nullptr) {
        std::cerr << "fatal error!, failed to get the module as the ptr of the module is nullptr!" << std::endl;
        throw std::runtime_error("fatal error!");
    }
    return *ptr;
}
}