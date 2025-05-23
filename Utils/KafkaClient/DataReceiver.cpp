#include "DataReceiver.hpp"
#include "Logger/LogMacroDef.hpp"
#include <cstring>

namespace CHAT::Utils::KafkaDataSender {

DataReceiver::DataReceiver(const std::string& topic,
                           const std::string& groupId,
                           const std::string& brokers,
                           std::shared_ptr<DataProcessor> processor)
    : m_topic(topic), m_groupId(groupId), m_brokers(brokers), m_processor(std::move(processor)) {}

DataReceiver::~DataReceiver() {
    close();
}

bool DataReceiver::open() {
    std::string errstr;
    m_conf = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
    if (!m_conf) {
        ERROR("KafkaReceiver", "Failed to create global config");
        return false;
    }

    m_callback = std::make_shared<ChatKafkaCallbackBase>();

    if (m_conf->set("bootstrap.servers", m_brokers, errstr) != RdKafka::Conf::CONF_OK) {
        ERROR("KafkaReceiver", "Failed to set brokers: " + errstr);
        return false;
    }

    if (m_conf->set("group.id", m_groupId, errstr) != RdKafka::Conf::CONF_OK) {
        ERROR("KafkaReceiver", "Failed to set group.id: " + errstr);
        return false;
    }

    if (m_conf->set("enable.auto.commit", "true", errstr) != RdKafka::Conf::CONF_OK) {
        ERROR("KafkaReceiver", "Failed to set enable.auto.commit: " + errstr);
        return false;
    }

    if (m_conf->set("auto.offset.reset", "earliest", errstr) != RdKafka::Conf::CONF_OK) {
        ERROR("KafkaReceiver", "Failed to set auto.offset.reset: " + errstr);
        return false;
    }

    m_conf->set("event_cb", static_cast<RdKafka::EventCb*>(m_callback.get()), errstr);
    m_conf->set("rebalance_cb", static_cast<RdKafka::RebalanceCb*>(m_callback.get()), errstr);

    m_consumer.reset(RdKafka::KafkaConsumer::create(m_conf.get(), errstr));
    if (!m_consumer) {
        ERROR("KafkaReceiver", "Failed to create consumer: " + errstr);
        return false;
    }

    std::vector<std::string> topics = {m_topic};
    RdKafka::ErrorCode err = m_consumer->subscribe(topics);
    if (err != RdKafka::ERR_NO_ERROR) {
        ERROR("KafkaReceiver", "Failed to subscribe: " + RdKafka::err2str(err));
        return false;
    }

    m_opened.store(true);
    INFO("KafkaReceiver", "Consumer started for topic: " + m_topic);
    return true;
}

void DataReceiver::close() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_consumer) {
        m_consumer->close();
        m_consumer.reset();
        m_callback.reset();
        m_opened.store(false);
        INFO("KafkaReceiver", "Consumer closed");
    }
}

void DataReceiver::pollMessages(int timeoutMs) {
    if (!m_opened || !m_consumer) return;

    std::unique_ptr<RdKafka::Message> msg(m_consumer->consume(timeoutMs));
    if (!msg) return;

    if (msg->err() == RdKafka::ERR_NO_ERROR) {
        const void* payload = msg->payload();
        size_t len = msg->len();
        std::string key = msg->key() ? *(msg->key()) : "";

        if (m_processor && payload && len > 0) {
            m_processor->process(key, reinterpret_cast<const uint8_t*>(payload), len);
        }

    } else if (msg->err() != RdKafka::ERR__TIMED_OUT) {
        ERROR("KafkaReceiver", "Consume error: " + msg->errstr());
    }
}

} // namespace CHAT::Utils::KafkaDataSender