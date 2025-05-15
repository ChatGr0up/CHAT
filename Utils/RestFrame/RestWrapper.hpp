#pragma once

#include "CommonDefine.hpp"
#include <string>
#include <vector>

namespace CHAT::Utils::RestFrame {   
class RestWrapper {
public:
    static RestWrapper& instance();

    void registerHandler(const std::string& className, const std::string& methodName, JsonHandler handler, const std::string& path);

    void loadRoutesFromConfig(); // only call when all rest methods have been registered!

    void startBySingle(); // only call when all rest methods have been registered!

private:
    RestWrapper() = default;

    ~RestWrapper() = default;

    std::vector<drogon::internal::HttpConstraint> getConstraintFromMethodVec(const std::vector<HttpMethod>& method);

    JsonHandler getHandlerFromCache(const std::string& className, const std::string& methodName);

    std::vector<HttpMethod> parseHttpMethod(const std::string& methodStr);

    bool isRouteValid(const std::string& route);

    void registerRoute(const std::string& path, const std::vector<HttpMethod>& method, JsonHandler handler);

private:
    std::string m_ipAddress{"0.0.0.0"};
    int m_port{8080};        
    HandlerCache m_handlerCache; 
};    
}