#include "RestWrapper.hpp"
#include <functional>
#include <exception>
#include "Logger/LogMacroDef.hpp"
#include "EnvManager/EnvManager.hpp"
#include <sstream>
#include <drogon/HttpResponse.h>

namespace CHAT::Utils::RestFrame {
RestWrapper &RestWrapper::instance()
{
    static RestWrapper instance;
    return instance;
}

RestWrapper::~RestWrapper()
{
    if (m_thread.joinable()) {
        drogon::app().quit();
        m_thread.join();
    }
}

std::vector<drogon::internal::HttpConstraint> RestWrapper::getConstraintFromMethodVec(const std::vector<HttpMethod> &method)
{
    std::vector<drogon::internal::HttpConstraint> httpConstraints;
    for (const auto& m : method) {
        httpConstraints.emplace_back(m);
    }
    TRACE("RestWrapper::getConstraintFromMethodVec", "http method size is " + std::to_string(httpConstraints.size()));
    return httpConstraints;
}

void RestWrapper::startBySingle()
{
    TRACE("RestWrapper::startBySingle", "now run the rest service， ip is " + m_ipAddress + ", port is " + std::to_string(m_port));
    drogon::app().setLogLevel(trantor::Logger::kTrace);  // 打开详细 log
    m_thread =  std::thread(&RestWrapper::asynRestThread, this);
    std::unique_lock<std::mutex> lock(m_startedMutex);
    if (!m_startedCond.wait_for(lock, std::chrono::seconds(10), [this]{ return m_isStarted;})) {
        ERROR("RestWrapper::startBySingle", "start timeout or exception!");
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if (!drogon::app().isRunning()) {
        ERROR("RestWrapper::startBySingle", "start failed!");
        return;
    } 
    TRACE("RestWrapper::startBySingle", "start success!");
}

void RestWrapper::asynRestThread()
{
    try {
        drogon::app().addListener(m_ipAddress, m_port);

        {
            std::lock_guard<std::mutex> lock(m_startedMutex);
            m_isStarted = true;
        }
        m_startedCond.notify_one();

        drogon::app().run(); // 这个是阻塞，直到服务退出
    } catch (const std::exception &ex) {
        ERROR("RestWrapper::asynRestThread", std::string("Exception: ") + ex.what());
        std::lock_guard<std::mutex> lock(m_startedMutex);
        m_isStarted = false;
        m_startedCond.notify_one();
    }
    TRACE("RestWrapper::asynRestThread", "rest server quit normally.");
}

bool RestWrapper::registerHandler(const std::string& methodName, JsonHandler handler, const std::string& path)
{
    if (!isRouteValid(path)) {
        ERROR("RestWrapper::registerHandler", "fatal error! check your param, path invalid");
        return false;
    }
    if (!handler) {
        ERROR("RestWrapper::registerHandler", "handler is empty on registration for path: " + path);
        return false;
    }
    HandlerCacheKey key{methodName, path};
    handlerCache.insert({key, handler});
    auto httpMethods = parseHttpMethod(methodName);
    auto lambda = [this, path, methodName](const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        TRACE("lambda of rest", "path is " + req->getPath());
        try {
            TRACE("lambda of rest", "fuck 1");
            if (handlerCache.find(std::make_pair(methodName, path)) == handlerCache.end()) {
                ERROR("lambda of rest", "fatal error, cannot find cache of handlers");
                return;
            }
            const auto& realHandler = handlerCache[std::make_pair(methodName, path)];
            TRACE("lambda of rest", "fuck 2");
            if (!realHandler) {
                ERROR("lambda of rest", "handler empty : " + path);
                return;
            }
            TRACE("lambda of rest", "fuck 3");
            auto method = req->getMethod();
            TRACE("lambda of rest", "fuck 4");
            std::shared_ptr<JsonValue> jsonPtr = *req;
            TRACE("lambda of rest", "fuck 5");
            std::string bodyStr = std::string(req->body());
            TRACE("lambda of rest", "fuck 6");
            TRACE("lambda of rest", "request body size: " + std::to_string( bodyStr.size()));
            if (method == drogon::HttpMethod::Post || method == drogon::HttpMethod::Put || method == drogon::HttpMethod::Patch) {
                constexpr size_t MAX_BODY_SIZE = 1024 * 1024;
                if (bodyStr.size() > MAX_BODY_SIZE) {
                    auto resp = drogon::HttpResponse::newHttpResponse();
                    resp->setStatusCode(drogon::k413RequestEntityTooLarge);
                    resp->setBody("Request body too large");
                    callback(resp);
                    return;
                }

                if (req->getContentType() != drogon::ContentType::CT_APPLICATION_JSON) {
                    auto resp = drogon::HttpResponse::newHttpResponse();
                    resp->setStatusCode(drogon::k400BadRequest);
                    resp->setContentTypeCode(drogon::ContentType::CT_TEXT_PLAIN);
                    resp->setBody("Expected application/json");
                    callback(resp);
                    return;
                }

                if (bodyStr.empty()) {
                    auto resp = drogon::HttpResponse::newHttpResponse();
                    resp->setStatusCode(drogon::k400BadRequest);
                    resp->setBody("Empty JSON body");
                    callback(resp);
                    return;
                }
                JsonValue jsonResponse = realHandler(*jsonPtr);
                auto httpResponse = drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
                httpResponse->setStatusCode(drogon::k200OK);
                httpResponse->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
                callback(httpResponse);
            } else {
                JsonValue emptyJson; 
                Json::JsonValue jsonRequest;
                TRACE("lambda of rest", "GET METHOD, body is " + std::string(bodyStr));
                TRACE("will call handler", "jsonRequest is " + std::string(bodyStr));
                JsonValue jsonResponse = realHandler(*jsonPtr);
                auto httpResponse = drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
                httpResponse->setStatusCode(drogon::k200OK);
                callback(httpResponse);
            }
        }
         catch (const std::bad_alloc& e) {
            ERROR("lambda of rest", "bad_alloc caught: " + std::string(e.what()));
            auto errorResponse = drogon::HttpResponse::newHttpResponse();
            errorResponse->setStatusCode(drogon::k500InternalServerError);
            errorResponse->setBody("Internal Server Error: Out of memory");
            callback(errorResponse);
            return;
        }
        catch (const std::exception& e) {
            TRACE("lambda of rest", "exception: " + std::string(e.what()));
            auto errorResponse = drogon::HttpResponse::newHttpResponse();
            errorResponse->setStatusCode(drogon::k500InternalServerError);
            errorResponse->setBody("Internal Server Error: " + std::string(e.what()));
            callback(errorResponse);
        }
        catch (...) {
            ERROR("lambda of rest", "unknown exception caught");
            auto errorResponse = drogon::HttpResponse::newHttpResponse();
            errorResponse->setStatusCode(drogon::k500InternalServerError);
            errorResponse->setBody("Internal Server Error: Unknown exception");
            callback(errorResponse);
            return;
        }
    };
    
    TRACE("RestWrapper::registerRoute", "registerHandler, path is " + path + ", method is " + methodName);
    drogon::app().registerHandler(path, lambda, getConstraintFromMethodVec(httpMethods));
    return true;
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
            TRACE("RestWrapper::parseHttpMethod", "method : " +  singleMethod);
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