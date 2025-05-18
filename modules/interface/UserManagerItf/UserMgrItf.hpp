#pragma once

#include "Module/ModuleBase.hpp"

namespace CHAT::Module {
class UserMgrItf : public CHAT::Utils::Module::ModuleBase {
public:
    UserMgrItf() = default;
    ~UserMgrItf() = default;
    std::string name() const override;
};
}