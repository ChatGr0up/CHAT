#include "RestWrapper.hpp"
#include <functional>
#include <exception>
#include "Logger/LogMacroDef.hpp"
#include "EnvManager/EnvManager.hpp"
#include <sstream>

namespace CHAT::Utils::RestFrame {
RestWrapper &RestWrapper::instance()
{
    static RestWrapper instance;
    return instance;
}

void RestWrapper::registerRoute(const std::string& path, const std::vector<HttpMethod>& method, JsonHandler handler)
{
    if (!isRouteValid(path)) {
        ERROR("RestWrapper::registerRoute", "path error! skip");
        return;
    }
    auto lambda = [handler](const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        try {
            JsonValue jsonRequest = Utils::Json::JsonUtils::fromString((std::string)req->body());

            JsonValue jsonResponse = handler(jsonRequest);

            auto httpResponse = drogon::HttpResponse::newHttpJsonResponse(jsonResponse);

            callback(httpResponse);

        } catch (const std::exception& e) {
            auto errorResponse = drogon::HttpResponse::newHttpResponse();
            errorResponse->setStatusCode(drogon::k400BadRequest);
            std::string errorMessage(e.what());
            errorResponse->setBody(std::string("Error: " + errorMessage));
            callback(errorResponse);
        }
    };
    drogon::app().registerHandler(path, lambda, getConstraintFromMethodVec(method));
}

std::vector<drogon::internal::HttpConstraint> RestWrapper::getConstraintFromMethodVec(const std::vector<HttpMethod> &method)
{
    std::vector<drogon::internal::HttpConstraint> httpConstraints;
    for (const auto& m : method) {
        httpConstraints.emplace_back(m);
    }
    return httpConstraints;
}

JsonHandler RestWrapper::getHandlerFromCache(const std::string& className, const std::string& methodName)
{
    if (m_handlerCache.empty()) {
        TRACE("RestWrapper::getHandlerFromCache", "handler cache is empry, directly return");
    }
    HandlerCacheKey handlerCacheKey(std::make_pair(className, methodName));
    if (m_handlerCache.find(handlerCacheKey) != m_handlerCache.end()) {
        return m_handlerCache[handlerCacheKey];
    }
    ERROR("RestWrapper::getHandlerFromCache", "fatal error!, failed to get handler from cache, check definition!");
    return JsonHandler();
}

void RestWrapper::startBySingle()
{
    drogon::app().addListener(m_ipAddress, m_port).run();
}

void RestWrapper::registerHandler(const std::string& className, const std::string& methodName, JsonHandler handler)
{
    HandlerCacheKey handlerCacheKey(std::make_pair(className, methodName));
    if (m_handlerCache.find(handlerCacheKey) != m_handlerCache.end()) {
        ERROR("RestWrapper::registerHandler", "the handler has been registered!");
        return;
    }
    m_handlerCache[handlerCacheKey] = handler;
}

void RestWrapper::loadRoutesFromConfig()
{
    std::string configPath = CHAT::Utils::EnvManager::EnvManager::getInstance().getGlobalConfigPath();
    std::string configFile = configPath + "/" + "RestRoutes.json";
    Utils::Json::JsonValue jsonValue = Utils::Json::JsonUtils::loadFromFile(configFile);
    for (const auto& routeDefine : jsonValue["routes"]) {
        std::string path = routeDefine["route_path"].asString();
        std::string restMethod = routeDefine["rest_method"].asString();
        std::string sourceClassName = routeDefine["source_class"].asString();
        std::string handlerName = routeDefine["handler_name"].asString();

        auto handler = getHandlerFromCache(sourceClassName, handlerName);
        if (!handler) {
            ERROR("RestWrapper::loadRoutesFromConfig", "fatal error! failed to get handler! skip");
            continue;
        }
        auto method = parseHttpMethod(restMethod);
        registerRoute(path, method, handler);
    }
    startBySingle();
}

std::vector<HttpMethod> RestWrapper::parseHttpMethod(const std::string& methodStr)
{
    std::vector<HttpMethod> httpMethods;
    if (methodStr.empty()) {
        ERROR("RestWrapper::parseHttpMethod", "fatal error! methodStr is empty");
        return httpMethods;
    }
    std::stringstream ss(methodStr);
    std::string singleMethod;
    static std::map<std::string, HttpMethod> strToEnum = {{"GET", drogon::HttpMethod::Get}, 
        {"POST", drogon::HttpMethod::Post}};
    while (std::getline(ss, singleMethod, ',')) {
        if (strToEnum.find(singleMethod) == strToEnum.end()) {
            ERROR("RestWrapper::parseHttpMethod", "fatal error! check your RestRoutes.json, http method invalid");
        } else {
            httpMethods.emplace_back(strToEnum[singleMethod]);
        }
    }
    return httpMethods;
}

bool RestWrapper::isRouteValid(const std::string &route)
{
   if (route.empty()) {
        return false; 
   }
   if (route[0] != '/') {
        return false;
   }
   return true;
}
}