#pragma once

#include <string>
#include <librdkafka/rdkafkacpp.h>
#include <memory>
#include <mutex>
#include <atomic>
#include "ChatKafkaCallbackBase.hpp"
#include "ChatKafkaCommon.hpp"

namespace CHAT::Utils::KafkaDataSender {

class DataSender {
public:
    explicit DataSender(const std::string& topic, const KafkaConfig& config);
    ~DataSender();

    bool open();
    void close();

    void sendFile(const std::string& key, const std::string& filePath);
    void sendMessage(const std::string& key, const std::vector<uint8_t>& data);

private:
    std::string m_topic;
    std::unique_ptr<RdKafka::Producer> m_producer;
    std::shared_ptr<ChatKafkaCallbackBase> m_callback;
    std::shared_ptr<RdKafka::Conf> m_conf;
    std::mutex m_mutex;
    std::atomic<bool> m_opened{false};
    std::once_flag onlyInitOnce;
    KafkaConfig m_kafkaConfig;
};

} // namespace CHAT::Utils::KafkaDataSender