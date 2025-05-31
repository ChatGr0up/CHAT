#pragma once

#include "Module/ModuleBase.hpp"
#include "JsonUtils/JsonUtils.hpp"

namespace CHAT::Module {
using JsonValue = CHAT::Utils::Json::JsonValue;
class UserMgrItf : public CHAT::Utils::Module::ModuleBase {
public:
    UserMgrItf() = default;

    ~UserMgrItf() = default;

    std::string name() const override;
    
    virtual JsonValue handleRequest(const JsonValue& request) = 0;
};
}