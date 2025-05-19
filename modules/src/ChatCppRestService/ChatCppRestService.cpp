#include "ChatCppRestService.hpp"
#include "RestFrame/RestWrapper.hpp"
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Module {
std::string ChatCppRestService::name() const 
{
    return std::string("ChatCppRestService");
}

JsonValue ChatCppRestService::testFunc(const JsonValue &req)
{
    Json::Value ret;
    ret["retCode"] = 0;
    ret["message"] = "Hello from testFunc!";
    return ret;
}

void ChatCppRestService::init() 
{
    TRACE("ChatCppRestService::init", "ChatCppRestService::init is called");
    registerHandler("ChatCppRestService", "GET", std::bind(&ChatCppRestService::testFunc, this, std::placeholders::_1), "/testFunc");
    startRestService();
}

void ChatCppRestService::registerHandler(const std::string& className, const std::string& methodName, Utils::RestFrame::JsonHandler handler, const std::string& path)
{
    TRACE("ChatCppRestService::registerHandler", "className " + className + " ,methodName " + methodName + " ,path " + path);
    CHAT::Utils::RestFrame::RestWrapper::instance().registerHandler(methodName, handler, path);
}

void ChatCppRestService::startRestService()
{
    CHAT::Utils::RestFrame::RestWrapper::instance().startBySingle();
}
}
extern "C" CHAT::Module::ChatCppRestServiceItf* createModule() {
    return new CHAT::Module::ChatCppRestService();
}