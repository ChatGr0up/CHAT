#include "RedisClient.hpp"
#include "Logger/LogMacroDef.hpp"
#include "EnvManager/EnvManager.hpp"
#include "JsonUtils/JsonUtils.hpp"

namespace CHAT::Utils::RedisClient {
RedisClient::RedisClient(const std::string& configName)
    : m_configName(configName)
{
    TRACE("RedisClient::RedisClient", "Initializing Redis client with config: " + m_configName);
    std::string configPath = Utils::EnvManager::EnvManager::getInstance().getRedisConfigPath();
    if (configPath.empty()) {
        ERROR("RedisClient::RedisClient", "Redis config path is empty, cannot initialize Redis client.");
        return;
    }
    TRACE("RedisClient::RedisClient", "Using Redis config path: " + configPath);
    configPath += "redisConfig.json";
    Utils::Json::JsonValue jsonValue = Utils::Json::JsonUtils::loadFromFile(configPath);
    if (jsonValue.isNull()) {
        ERROR("RedisClient::RedisClient", "Failed to load Redis configuration from file: " + configPath);
        return; 
    }
    if (!jsonValue.isMember("configName")) {
        ERROR("RedisClient::RedisClient", "Redis configuration does not contain 'configName' field.");
        return;
    }
    jsonValue = jsonValue[m_configName];
    sw::redis::ConnectionOptions opts;
    opts.host = jsonValue["host"].asString();
    opts.port = jsonValue["port"].asInt();
    opts.password = jsonValue["password"].asString();
    opts.db = jsonValue["db"].asInt();
    opts.socket_timeout = std::chrono::milliseconds(jsonValue["timeout_ms"].asInt());
    opts.keep_alive = jsonValue["keep_alive"].asBool();
    if (jsonValue["ssl"].asBool()) {
        opts.tls.enabled = true;
        auto tlsConfig = jsonValue["tls"];
        opts.tls.enabled = true;
        opts.tls.cert = tlsConfig["client_cert"].asString();     
        opts.tls.key = tlsConfig["client_key"].asString();        
        opts.tls.cacert = tlsConfig["ca_cert"].asString();        
        opts.tls.sni = tlsConfig["sni"].asString();  
    }

    sw::redis::ConnectionPoolOptions pool_opts;
    if (jsonValue.isMember("pool")) {
        pool_opts.size = jsonValue["pool"]["size"].asUInt();  // 最大连接数
        pool_opts.wait_timeout = std::chrono::milliseconds(jsonValue["pool"]["wait_timeout_ms"].asInt());  // 阻塞等待时间
    } else {
        pool_opts.size = 5;
        pool_opts.wait_timeout = std::chrono::milliseconds(100);
    }
    
    try {
        m_redis = std::make_unique<sw::redis::Redis>(opts, pool_opts);
    } catch (const sw::redis::Error& e) {
        ERROR("RedisClient::RedisClient", "Failed to initialize Redis client: " + std::string(e.what()));
    }
}
}