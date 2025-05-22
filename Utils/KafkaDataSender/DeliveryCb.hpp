#pragma once

#include <librdkafka/rdkafkacpp.h>

namespace CHAT::Utils::KafkaDataSender {
class DeliveryCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb(RdKafka::Message& message) override;
};
}