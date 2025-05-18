#pragma once

#include "ChatCppRestServiceItf/ChatCppRestServiceItf.hpp"
#include "RestFrame/CommonDefine.hpp"

namespace CHAT::Module {
using JsonValue = CHAT::Utils::Json::JsonValue;
class ChatCppRestService : public Module::ChatCppRestServiceItf, 
    public drogon::HttpController<ChatCppRestService> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ChatCppRestService::handleHello, "/hello", drogon::Get);
    METHOD_LIST_END
            
public:
    ChatCppRestService() = default;

    ~ChatCppRestService();

    ChatCppRestService(const ChatCppRestService&) = delete;

    ChatCppRestService& operator=(const ChatCppRestService&) = delete;

    std::string name() const override;

    void init() override;

    void registerHandler(const std::string& className, const std::string& methodName, 
        Utils::RestFrame::JsonHandler handler, const std::string& path);
        
public:
    void handleHello(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
};
}