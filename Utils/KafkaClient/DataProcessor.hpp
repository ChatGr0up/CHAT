#pragma once

#include <string>
#include <cstdint>
#include <cstddef>

namespace CHAT::Utils::KafkaDataSender {

class DataProcessor {
public:
    virtual ~DataProcessor() = default;

    /**
     * @brief 处理从 Kafka 接收到的消息。
     * @param key Kafka 消息的键
     * @param data 消息数据指针
     * @param length 数据长度（字节）
     */
    virtual void process(const std::string& key, const uint8_t* data, size_t length) = 0;
};

} // namespace CHAT::Utils::KafkaDataSender