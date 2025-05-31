#pragma once

#include "Logger/LogMacroDef.hpp"
#include <string>
#include <map>

namespace CHAT::Utils::KafkaDataSender {
using KafkaConfig = std::map<std::string, std::string>;

void setValueIfAbsent(KafkaConfig& config, const std::string& key, const std::string& defaultVal)
{
    if (config.find(key) != config.end()) {
        config[key] = defaultVal;
        TRACE("KafkaDataSender::setValueIfAbsent", "key is " + key + ", val is " + defaultVal);
    }
    TRACE("KafkaDataSender::setValueIfAbsent", "key is " + key + ", val is " + config[key]);    
}
}