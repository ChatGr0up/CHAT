#pragma once 

#include "drogon/drogon.h"
#include "JsonUtils/JsonUtils.hpp"

namespace CHAT::Utils::RestFrame {
using HttpMethod = drogon::HttpMethod;    
using JsonValue = CHAT::Utils::Json::JsonValue;
using JsonHandler = std::function<JsonValue(const JsonValue&)>;
using HandlerCacheKey = std::pair<std::string, std::string>;
using HandlerCache = std::map<HandlerCacheKey, JsonHandler>;
}