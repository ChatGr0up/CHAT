#pragma once
#include <librdkafka/rdkafkacpp.h>
#include "ChatDeliveryCb.hpp"
#include "ChatEventCb.hpp"
#include "ChatPartitionerCb.hpp"
#include "ChatRebalanceCb.hpp"
#include <string>
#include <vector>

namespace CHAT::Utils::KafkaDataSender {

class ChatKafkaCallbackBase : public ChatEventCb,
                               public ChatDeliveryCb,
                               public ChatRebalanceCb,
                               public ChatPartitionerCb {
public:
    virtual ~ChatKafkaCallbackBase() = default;

    // Kafka producer: 投递报告回调
    void dr_cb(RdKafka::Message& message) override;

    // Kafka runtime 事件处理（error, stats, log）
    void event_cb(RdKafka::Event& event) override;

    // Kafka consumer: 消费组 rebalance 回调
    void rebalance_cb(RdKafka::KafkaConsumer* consumer,
                      RdKafka::ErrorCode err,
                      std::vector<RdKafka::TopicPartition*>& partitions) override;

    // Kafka producer: 自定义分区选择回调
    int32_t partitioner_cb(const RdKafka::Topic* topic,
                           const std::string* key,
                           int32_t partition_cnt,
                           void* msg_opaque) override;
};

} // namespace CHAT::Utils::KafkaDataSender