#include "ChatCppRestService.hpp"
#include "RestFrame/RestWrapper.hpp"
#include "Logger/LogMacroDef.hpp"
#include "Module/ModuleUtils.hpp"
#include "UserManagerItf/UserMgrItf.hpp"

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
    registerHandler("ChatCppRestService", "POST", std::bind(&ChatCppRestService::userManagement, this, std::placeholders::_1), "/v1/usermanagement");
    registerHandler("ChatCppRestService", "POST", std::bind(&ChatCppRestService::sendRegisterEmailVerifyCode, this, std::placeholders::_1), "/v1/sendRegisterEmailVerifyCode");
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

JsonValue ChatCppRestService::userManagement(const JsonValue& req)
{
    CHAT::Module::UserMgrItf& userMgrItf = CHAT::Utils::Module::getModuleRef<CHAT::Module::UserMgrItf>();
    return userMgrItf.handleRequest(req);
}

JsonValue ChatCppRestService::sendRegisterEmailVerifyCode(const JsonValue &req)
{
    CHAT::Module::UserMgrItf& userMgrItf = CHAT::Utils::Module::getModuleRef<CHAT::Module::UserMgrItf>();
    return userMgrItf.sendRegisterEmailVerifyCode(req);
}
}
extern "C" CHAT::Module::ChatCppRestServiceItf* createModule() {
    return new CHAT::Module::ChatCppRestService();
}