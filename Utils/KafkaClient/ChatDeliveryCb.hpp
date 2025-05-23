#pragma once

#include <librdkafka/rdkafkacpp.h>

namespace CHAT::Utils::KafkaDataSender {
class ChatDeliveryCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb(RdKafka::Message& message) override;

    ~ChatDeliveryCb() override = default;
};
}