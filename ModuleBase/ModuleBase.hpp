
#pragma once

#include <string>

namespace CHAT::Module{
class ModuleBase {
public:
    virtual ~ModuleBase() = default;
    virtual void load() = 0;  // 每个模块必须实现的加载方法
    virtual std::string name() const = 0;  // 返回模块名称
private:
    std::string m_ModuleName;      
};
}
