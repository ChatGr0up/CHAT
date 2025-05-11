#include "ChatCppRestService.hpp"
#include "RestFrame/RestWrapper.hpp"
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Module {
std::string ChatCppRestService::name() const 
{
    return std::string("ChatCppRestService");
}

void ChatCppRestService::init() 
{
    TRACE("ChatCppRestService::init", "now register the rest mothods and run!");
    loadRoutesFromConfig();
}

void ChatCppRestService::loadRoutesFromConfig()
{
    CHAT::Utils::RestFrame::RestWrapper::instance().loadRoutesFromConfig();
}

void ChatCppRestService::registerHandler(const std::string& className, const std::string& methodName, 
    Utils::RestFrame::JsonHandler handler)
{
    CHAT::Utils::RestFrame::RestWrapper::instance().registerHandler(className, methodName, handler);
}
}
extern "C" CHAT::Module::ChatCppRestServiceItf* createModule() {
    return new CHAT::Module::ChatCppRestService();
}