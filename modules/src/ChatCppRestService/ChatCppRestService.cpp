#include "ChatCppRestService.hpp"
#include "RestFrame/RestWrapper.hpp"
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Module {
std::string ChatCppRestService::name() const 
{
    return std::string("ChatCppRestService");
}

ChatCppRestService::~ChatCppRestService()
{
    TRACE("ChatCppRestService::~ChatCppRestService", "ChatCppRestService destructor!");
}

void ChatCppRestService::init() 
{
    sendMessage();
    // make sure to call it after all rest methods have been registered
    TRACE("ChatCppRestService::init", "now register the rest mothods and run!");
    // loadRoutesFromConfig();
    CHAT::Utils::RestFrame::RestWrapper::instance().startBySingle();
}

void ChatCppRestService::loadRoutesFromConfig()
{
    CHAT::Utils::RestFrame::RestWrapper::instance().loadRoutesFromConfig();
}

void ChatCppRestService::sendMessage()
{
    Utils::RestFrame::JsonHandler operate = [](const CHAT::Utils::Json::JsonValue& request) -> CHAT::Utils::Json::JsonValue {
        CHAT::Utils::Json::JsonValue result;
        result["result"] = "ok";
        result["message"] = "succeffully called";
        return result;
    };
    std::string path = "/chatcpp-0/v1/hello";
    registerHandler("ChatCppRestService", "POST", operate, path);
}

void ChatCppRestService::registerHandler(const std::string& className, const std::string& methodName, 
    Utils::RestFrame::JsonHandler handler, const std::string& path)
{
    TRACE("debug", "1");
    CHAT::Utils::RestFrame::RestWrapper::instance().registerHandler(className, methodName, handler, path);
}
}
extern "C" CHAT::Module::ChatCppRestServiceItf* createModule() {
    return new CHAT::Module::ChatCppRestService();
}