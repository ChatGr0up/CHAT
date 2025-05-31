#include "DataSender.hpp"
#include "Logger/LogMacroDef.hpp"
#include <fstream>
#include <sstream>
#include <memory>
#include "ChatKafkaCallbackBase.hpp"

namespace CHAT::Utils::KafkaDataSender {

DataSender::DataSender(const std::string& topic, const KafkaConfig& config)
    : m_topic(topic), m_kafkaConfig(config) {}

DataSender::~DataSender() {
    close();
}

bool DataSender::open() {
    std::string errstr;
    std::unique_ptr<RdKafka::Conf> conf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));

    if (conf->set("bootstrap.servers", "localhost:9092", errstr) != RdKafka::Conf::CONF_OK) {
        ERROR("Kafka", "Failed to set broker: " + errstr);
        return false;
    }

    setValueIfAbsent(m_kafkaConfig, "message.max.bytes", "1000000");

    for (const auto& entry : m_kafkaConfig) {
        if (conf->set(entry.first, entry.second, errstr) != RdKafka::Conf::CONF_OK) {
            ERROR("Kafka", "Failed to set : " + entry.first + ", and errstr is " + errstr);
        }
    }
    m_callback = std::make_unique<ChatKafkaCallbackBase>();

    conf->set("event_cb", static_cast<RdKafka::EventCb*>(m_callback.get()), errstr);
    conf->set("dr_cb", static_cast<RdKafka::DeliveryReportCb*>(m_callback.get()), errstr);
    conf->set("partitioner_cb", static_cast<RdKafka::PartitionerCb*>(m_callback.get()), errstr);

    RdKafka::Producer* rawProducer = RdKafka::Producer::create(conf.get(), errstr);
    if (!rawProducer) {
        ERROR("Kafka", "Failed to create producer: " + errstr);
        return false;
    }
    m_producer.reset(rawProducer);

    INFO("Kafka", "Producer created successfully");
    return true;
}

void DataSender::close() {
    if (m_producer) {
        INFO("Kafka", "Flushing remaining messages");
        m_producer->flush(3000);
        m_producer.reset();
        m_callback.reset();
    }
}

void DataSender::sendFile(const std::string& key, const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        ERROR("Kafka", "Failed to open file: " + filePath);
        return;
    }
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        ERROR("Kafka", "Failed to read file: " + filePath);
        return;
    }
    sendMessage(key, buffer);
}

void DataSender::sendMessage(const std::string& key, const std::vector<uint8_t>& data) {
    if (!m_producer) {
        ERROR("Kafka", "Producer not initialized");
        return;
    }
    RdKafka::ErrorCode resp = m_producer->produce(
        m_topic,
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<uint8_t*>(data.data()),
        data.size(),
        key.data(),
        key.size(),
        0,
        nullptr,
        nullptr);

    if (resp != RdKafka::ERR_NO_ERROR) {
        ERROR("Kafka", "Failed to produce message: " + RdKafka::err2str(resp));
    } else {
        TRACE("Kafka", "Message enqueued successfully, size: " + std::to_string(data.size()));
    }

    m_producer->poll(0);
}

}  // namespace CHAT::Utils::KafkaDataSender