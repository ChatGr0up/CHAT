#pragma once

#include "CommonDefine.hpp"
#include <string>
#include <vector>

namespace CHAT::Utils::RestFrame {   
class RestWrapper {
public:
    static RestWrapper& instance();

    bool registerHandler(const std::string& methodName, JsonHandler handler, const std::string& path);

    void startBySingle(); // only call when all rest methods have been registered!

private:
    RestWrapper() = default;

    ~RestWrapper();

    std::vector<drogon::internal::HttpConstraint> getConstraintFromMethodVec(const std::vector<HttpMethod>& method);

    std::vector<HttpMethod> parseHttpMethod(const std::string& methodStr);

    bool isRouteValid(const std::string& route);

    void asynRestThread();

private:
    std::string m_ipAddress{"0.0.0.0"};
    int m_port{6789};        
    std::thread m_thread;
    std::mutex m_startedMutex;
    std::condition_variable m_startedCond;
    bool m_isStarted{false};
};    
}