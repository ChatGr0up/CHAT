#pragma once

#include <string>
#include <sw/redis++/redis++.h>

namespace CHAT::Utils::RedisClient {
using namespace sw::redis;
class RedisClient {
public:
    RedisClient(const std::string& configName = "default");
    ~RedisClient();
private:
    std::string m_configName;
    std::unique_ptr<Redis> m_redis;
};
}