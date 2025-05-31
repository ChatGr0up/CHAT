#pragma once

#include "UserManagerItf/UserMgrItf.hpp"
#include "RequestHandler.hpp"
#include <iostream>

namespace CHAT::Module {
class UserMgr : public CHAT::Module::UserMgrItf {
public:
    UserMgr() = default;

    ~UserMgr() = default;

    void init() override;

    std::string name() const override;

    JsonValue handleRequest(const JsonValue& request) override;
private:
    bool isAyncRequest(const std::string& requestType);

    JsonValue handleRequestAsync(const JsonValue &request);

    JsonValue handleRequestSync(const JsonValue &request);

    JsonValue defaultReply(const JsonValue &request);
private:
    std::unique_ptr<RequestHandler> m_requestHandler{nullptr};
};
}
