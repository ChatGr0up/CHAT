#pragma once

#include <librdkafka/rdkafkacpp.h>

namespace CHAT::Utils::KafkaDataSender {
class ChatEventCb : public RdKafka::EventCb {
public:
    void event_cb(RdKafka::Event& event) override;
};
}