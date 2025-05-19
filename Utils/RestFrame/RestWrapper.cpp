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
    drogon::app().setLogLevel(trantor::Logger::kTrace);
    m_thread =  std::thread(&RestWrapper::asynRestThread, this);
    std::unique_lock<std::mutex> lock(m_startedMutex);

    m_startedCond.wait(lock, [this]{ return m_isStarted;});

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

        drogon::app().run(); // 阻塞函数
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
    auto httpMethods = std::move(parseHttpMethod(methodName));
    if (httpMethods.empty()) {
        ERROR("RestWrapper::registerHandler", "fatal error! check your param, http method invalid");
        return false;
    }
    auto lambda = [handler](const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        TRACE("lambda of rest", "request path is " + req->getPath() + ", now call the handler!");
        try {
            JsonValue jsonResponse = handler(*req->jsonObject());
            auto httpResponse = drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
            httpResponse->setStatusCode(drogon::k200OK);
            callback(httpResponse);
        }catch(...){
            return;
        }
    };
    TRACE("RestWrapper::registerRoute", "registerHandler, path is " + path + ", method is " + methodName);
    drogon::app().registerHandler(path, std::move(lambda), getConstraintFromMethodVec(httpMethods));
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