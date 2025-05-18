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
    TRACE("ChatCppRestService::init", "ChatCppRestService::init is called");
}

void ChatCppRestService::registerHandler(const std::string& className, const std::string& methodName, Utils::RestFrame::JsonHandler handler, const std::string& path)
{
    TRACE("ChatCppRestService::registerHandler", "className " + className + " ,methodName " + methodName + " ,path " + path);
    CHAT::Utils::RestFrame::RestWrapper::instance().registerHandler(methodName, handler, path);
}

void ChatCppRestService::handleHello(const drogon::HttpRequestPtr& req, 
    std::function<void(const drogon::HttpResponsePtr&)>&& callback)
{
    Json::Value ret;
    ret["retCode"] = 0;
    ret["message"] = "Hello from controller!";
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

}
extern "C" CHAT::Module::ChatCppRestServiceItf* createModule() {
    return new CHAT::Module::ChatCppRestService();
}