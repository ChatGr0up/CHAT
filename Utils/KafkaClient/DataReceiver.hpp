#pragma once

#include <string>
#include <librdkafka/rdkafkacpp.h>
#include <memory>
#include <mutex>
#include <atomic>
#include "ChatKafkaCallbackBase.hpp"
#include "DataProcessor.hpp"

namespace CHAT::Utils::KafkaDataSender {

class DataReceiver {
public:
    DataReceiver(const std::string& topic,
                 const std::string& groupId,
                 const std::string& brokers,
                 std::shared_ptr<DataProcessor> processor);

    ~DataReceiver();

    bool open();
    void close();

    void pollMessages(int timeoutMs = 1000);

private:
    std::string m_topic;
    std::string m_groupId;
    std::string m_brokers;
    std::shared_ptr<DataProcessor> m_processor;

    std::shared_ptr<RdKafka::KafkaConsumer> m_consumer;
    std::shared_ptr<ChatKafkaCallbackBase> m_callback;
    std::shared_ptr<RdKafka::Conf> m_conf;
    std::mutex m_mutex;
    std::atomic<bool> m_opened{false};
};

} // namespace CHAT::Utils::KafkaDataSender