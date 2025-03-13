#include "../Utils/Module/ModuleLoader.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <dlfcn.h>


int main() {
    CHAT::Utils::Module::ModuleLoader loader;
    loader.loadModules({"moduleA", "moduleB"});  // 需要加载的模块
    return 0;
}
