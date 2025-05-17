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
    registerHandler("ChatCppRestService", "GET", std::bind(&ChatCppRestService::handleHello, this, std::placeholders::_1), "/chat/v1/chatcpp-0/hello");
}

void ChatCppRestService::registerHandler(const std::string& className, const std::string& methodName, Utils::RestFrame::JsonHandler handler, const std::string& path)
{
    // TRACE("ChatCppRestService::registerHandler", "className " + className + " ,methodName " + methodName + " ,path " + path);
    CHAT::Utils::RestFrame::RestWrapper::instance().registerHandler(methodName, handler, path);
}

JsonValue ChatCppRestService::handleHello(const JsonValue& jsonRequest)
{
    TRACE("ChatCppRestService::handleHello", "ChatCppRestService::handleHello is called");
    // CHAT::Utils::Json::JsonValue result;
    // result["result"] = "ok";
    // result["message"] = "succeffully called";
    return JsonValue();
}
}
extern "C" CHAT::Module::ChatCppRestServiceItf* createModule() {
    return new CHAT::Module::ChatCppRestService();
}