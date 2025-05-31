#include "UserMgr.hpp"
#include "Logger/LogMacroDef.hpp"
#include "JsonUtils/JsonUtils.hpp"
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Module {
void UserMgr::init()
{
    TRACE("UserMgr::init", "called init");
    m_requestHandler = std::make_unique<RequestHandler>();
}

std::string UserMgr::name() const 
{
    return std::string("UserMgr");
}

JsonValue UserMgr::handleRequest(const JsonValue &request)
{
    JsonValue result;
    std::string requestType = Utils::Json::JsonUtils::toString(request["requestType"]);
    if (isAyncRequest(requestType)) {
        return handleRequestAsync(request);
    }
    return handleRequestSync(request);
}

bool UserMgr::isAyncRequest(const std::string& requestType)
{
    // todo based on requestType;
    return true;
}

JsonValue UserMgr::handleRequestAsync(const JsonValue &request)
{
    if (m_requestHandler == nullptr) {
        ERROR("UserMgr::handleRequestAsync", "fatal error! m_requestHandler is nullptr");
        return defaultReply(request);
    }
    return m_requestHandler->handleRequestAsync(request);
}

JsonValue UserMgr::handleRequestSync(const JsonValue &request)
{
    if (m_requestHandler == nullptr) {
        ERROR("UserMgr::handleRequestAsync", "fatal error! m_requestHandler is nullptr");
        return defaultReply(request);
    }
    return m_requestHandler->handleRequestSync(request);
}

JsonValue UserMgr::defaultReply(const JsonValue &request)
{
    // todo: return fetch reply for customed requestType;
    JsonValue returnValue;
    returnValue["ret"] = -1;
    returnValue["message"] = "something is error!";
}

}
extern "C" CHAT::Module::UserMgrItf* createModule() {
    return new CHAT::Module::UserMgr();
}