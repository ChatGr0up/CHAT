#pragma once

#include "UserManagerItf/UserMgrItf.hpp"
#include <iostream>

namespace CHAT::Module {
class UserMgr : public CHAT::Module::UserMgrItf {
public:
    UserMgr() = default;

    ~UserMgr() = default;

    void init() override;

    std::string name() const override;

    JsonValue handleRequest(const JsonValue& request) override;
};
}
