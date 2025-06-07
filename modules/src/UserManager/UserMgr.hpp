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

    JsonValue sendRegisterEmailVerifyCode(const JsonValue& req);
private:
    bool isAyncRequest(const std::string& type);

    JsonValue handleRequestAsync(const JsonValue &request);

    JsonValue handleRequestSync(const JsonValue &request);

    JsonValue defaultReply(const JsonValue &request);

    bool isValidEmail(const std::string& email);

    std::string toSendRegisterEmailVerifyCode(const std::string& email);

    bool storeCodeToCahce(const std::string& email, const std::string& code);
private:
    std::unique_ptr<RequestHandler> m_requestHandler{nullptr};
};
}
