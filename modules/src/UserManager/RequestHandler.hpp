#pragma once

#include "Thread/AsyncTask.hpp"
#include "JsonUtils/JsonUtils.hpp"

namespace CHAT::Module {
using AsyncTask = CHAT::Utils::Thread::AsyncTask;
using JsonValue = CHAT::Utils::Json::JsonValue;
class RequestHandler : public AsyncTask {
public:
    JsonValue handleRequestAsync(const JsonValue &request);

    JsonValue handleRequestSync(const JsonValue &request);
protected:
    uint32_t loopInterval() override; 
    
    uint32_t loopDelayTime() override; 

    bool svc() override; 
private:
    std::vector<JsonValue> m_requestQueue;
};
}