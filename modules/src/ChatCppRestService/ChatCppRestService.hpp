#pragma once

#include "ChatCppRestServiceItf/ChatCppRestServiceItf.hpp"
#include "RestFrame/CommonDefine.hpp"

namespace CHAT::Module {
class ChatCppRestService : public Module::ChatCppRestServiceItf {
public:
    ChatCppRestService() = default;

    ~ChatCppRestService();

    ChatCppRestService(const ChatCppRestService&) = delete;

    ChatCppRestService& operator=(const ChatCppRestService&) = delete;

    std::string name() const override;

    void init() override;

    void registerHandler(const std::string& className, const std::string& methodName, 
        Utils::RestFrame::JsonHandler handler, const std::string& path);

private:
    void loadRoutesFromConfig();

    void sendMessage();
};
}