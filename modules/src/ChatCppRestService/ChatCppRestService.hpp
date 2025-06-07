#pragma once

#include "ChatCppRestServiceItf/ChatCppRestServiceItf.hpp"
#include "RestFrame/CommonDefine.hpp"

namespace CHAT::Module {
using JsonValue = CHAT::Utils::Json::JsonValue;
class ChatCppRestService : public Module::ChatCppRestServiceItf {
public:
    ChatCppRestService() = default;

    ~ChatCppRestService() override = default;

    ChatCppRestService(const ChatCppRestService&) = delete;

    ChatCppRestService& operator=(const ChatCppRestService&) = delete;

    std::string name() const override;

    void init() override;

    void registerHandler(const std::string& className, const std::string& methodName, 
     Utils::RestFrame::JsonHandler handler, const std::string& path);
private:
    JsonValue testFunc(const JsonValue& req);

    JsonValue userManagement(const JsonValue& req);

    JsonValue sendRegisterEmailVerifyCode(const JsonValue& req);

    void startRestService();
};
}