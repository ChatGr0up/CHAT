#include "RestWrapper.hpp"
#include <functional>
#include <exception>
#include "Logger/LogMacroDef.hpp"
#include "EnvManager/EnvManager.hpp"
#include <sstream>
#include <drogon/HttpResponse.h>
#include "FileUtils/fileSystem.hpp"
#include "drogon/HttpTypes.h"

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
        {
            std::lock_guard<std::mutex> lock(m_startedMutex);
            m_isStarted = true;
        }
        m_startedCond.notify_one();
        std::string configPath = CHAT::Utils::EnvManager::EnvManager::getInstance().getRestServiceConfigPath();
        if (configPath.empty()) {
            ERROR("RestWrapper::asynRestThread", " failed to get rest config file path, use m_port : " + std::to_string(m_port));
            drogon::app().addListener(m_ipAddress, m_port);
            drogon::app().run(); // 阻塞函数
        } else {
            configPath += "/DrogonRestConfig.json";
            TRACE("RestWrapper::asynRestThread", "rest config path is " + configPath);
            if (!Utils::FileUtils::fileSystem::fileExists(configPath)) {
                ERROR("RestWrapper::asynRestThread", "fatal error, file is not exists! " + configPath);
            }
            try {
                drogon::app().loadConfigFile(configPath);
            } catch(...) {
                ERROR("RestWrapper::asynRestThread", "failed to get configFile " + configPath);
                drogon::app().addListener(m_ipAddress, m_port);
            }
            drogon::app().run(); // 阻塞函数
        }
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
        if (req->method() == drogon::HttpMethod::Options) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::k200OK);
            resp->addHeader("Access-Control-Allow-Origin", "*");
            resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
            callback(resp);
            return;
        }
        try {
            JsonValue jsonResponse = handler(*req->jsonObject());
            auto httpResponse = drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
            httpResponse->setStatusCode(drogon::k200OK);
            if (jsonResponse.isMember("status")) {
                httpResponse->setStatusCode(static_cast<drogon::HttpStatusCode>(jsonResponse["status"].asInt()));
            }
            httpResponse->addHeader("Access-Control-Allow-Origin", "*");
            httpResponse->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            httpResponse->addHeader("Access-Control-Allow-Headers", "Content-Type");

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
        {"POST", drogon::HttpMethod::Post}, {"OPTIONS", drogon::HttpMethod::Options}, 
        {"PUT", drogon::HttpMethod::Put}, {"DELETE", drogon::HttpMethod::Delete}, 
        {"PATCH", drogon::HttpMethod::Patch}, {"HEAD", drogon::HttpMethod::Head}};
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