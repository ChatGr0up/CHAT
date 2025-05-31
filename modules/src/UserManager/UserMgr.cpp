#include "UserMgr.hpp"
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Module {
void UserMgr::init()
{
    TRACE("UserMgr::init", "called init");
}

std::string UserMgr::name() const 
{
    return std::string("UserMgr");
}

JsonValue UserMgr::handleRequest(const JsonValue &request)
{
    JsonValue result;
    result["ret"] = 0;
    result["message"] = "Hello from handleRequest!";
    return result;
}
}
extern "C" CHAT::Module::UserMgrItf* createModule() {
    return new CHAT::Module::UserMgr();
}