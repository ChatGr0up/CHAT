#pragma once

#include <librdkafka/rdkafkacpp.h>
#include <string>

namespace CHAT::Utils::KafkaDataSender {

class ChatPartitionerCb : public RdKafka::PartitionerCb {
public:
    int32_t partitioner_cb(const RdKafka::Topic* topic,
                           const std::string* key,
                           int32_t partition_cnt,
                           void* msg_opaque) override;

    ~ChatPartitionerCb() override = default;
};

}  // namespace CHAT::Utils::KafkaDataSender