#include "UserMgr.hpp"
#include "Logger/LogMacroDef.hpp"
#include "JsonUtils/JsonUtils.hpp"
#include "Logger/LogMacroDef.hpp"
#include "CommonDefine.hpp"
#include "RestFrame/CommonDefine.hpp"
#include "CommonUtils/RandomCode.hpp"
#include <regex>
#include <random>

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

bool UserMgr::isAyncRequest(const std::string& type)
{
    if (type.empty()) {
        ERROR("UserMgr::isAyncRequest", "fatal error! requestType is empty");
        return false;
    }
    auto requestType = static_cast<CHAT::Module::RestDefine::RequestType>(std::stoi(type));
    if (requestType == CHAT::Module::RestDefine::RequestType::UserLogin ||
        requestType == CHAT::Module::RestDefine::RequestType::UserLogout ||
        requestType == CHAT::Module::RestDefine::RequestType::UserRegister ||
        requestType == CHAT::Module::RestDefine::RequestType::UserInfoUpdate ||
        requestType == CHAT::Module::RestDefine::RequestType::UserInfoFetch ||
        requestType == CHAT::Module::RestDefine::RequestType::UserPasswordChange ||
        requestType == CHAT::Module::RestDefine::RequestType::UserPasswordReset ||
        requestType == CHAT::Module::RestDefine::RequestType::UserFriendAdd ||
        requestType == CHAT::Module::RestDefine::RequestType::UserFriendRemove ||
        requestType == CHAT::Module::RestDefine::RequestType::UserFriendListFetch ||
        requestType == CHAT::Module::RestDefine::RequestType::UserGroupCreate ||
        requestType == CHAT::Module::RestDefine::RequestType::UserGroupJoin ||
        requestType == CHAT::Module::RestDefine::RequestType::UserGroupLeave ||
        requestType == CHAT::Module::RestDefine::RequestType::UserGroupListFetch ||
        requestType == CHAT::Module::RestDefine::RequestType::UserGroupMessageSend ||
        requestType == CHAT::Module::RestDefine::RequestType::UserGroupMessageFetch) {
            return false;
    }
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


JsonValue UserMgr::sendRegisterEmailVerifyCode(const JsonValue& req)
{
    std::string email = Utils::Json::JsonUtils::toString(req["email"]);
    if (email.empty()) {
        ERROR("UserMgr::sendRegisterEmailVerifyCode", "fatal error! email is empty");
        JsonValue ret;
        ret["ret"] = -1;
        ret["status"] = drogon::HttpStatusCode::k400BadRequest;
        ret["message"] = "email is empty";
        return ret;
    }
    if (!isValidEmail(email)) {
        ERROR("UserMgr::sendRegisterEmailVerifyCode", "fatal error! email is invalid");
        JsonValue ret;
        ret["ret"] = -1;
        ret["status"] = drogon::HttpStatusCode::k400BadRequest;
        ret["message"] = "email is invalid";
        return ret;
    }
    std::string code = toSendRegisterEmailVerifyCode(email);
    if (!storeCodeToCahce(email, code)) {
        ERROR("UserMgr::sendRegisterEmailVerifyCode", "fatal error! store code to cache failed");
        JsonValue ret;
        ret["ret"] = -1;
        ret["status"] = drogon::HttpStatusCode::k500InternalServerError;
        ret["message"] = "store code to cache failed";
        return ret;
    }
    
    JsonValue ret;
    ret["ret"] = 0;
    ret["message"] = "send register email verify code success";
    return ret;
}

bool UserMgr::isValidEmail(const std::string& email)
{
    const std::regex pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    return std::regex_match(email, pattern);
}

std::string UserMgr::toSendRegisterEmailVerifyCode(const std::string& email)
{
    std::string code = CHAT::Utils::CommonUtils::RandomCode::generateVerifyCodeStr(email);

    TRACE("UserMgr::toSendRegisterEmailVerifyCode", "Generated code: " + code + " for email: " + email);

    
}

bool UserMgr::storeCodeToCahce(const std::string& email, const std::string& code)
{
    TRACE("UserMgr::storeCodeToCahce", "store code to cache success for email: " + email);
    // todo 需要引入redis
    return true;
}

} // namespace CHAT::Module
extern "C" CHAT::Module::UserMgrItf* createModule() {
    return new CHAT::Module::UserMgr();
}