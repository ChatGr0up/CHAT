#include "DeliveryCb.hpp"
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Utils::KafkaDataSender {
void DeliveryCb::dr_cb(RdKafka::Message& message)
{
    if (message.err()) {
        ERROR("DeliveryCb::dr_cb", "Message delivery failed");
        return;
    }
    TRACE("DeliveryCb::dr_cb", "Message delivery to topic " + message.topic_name() + 
        ", partition [" + std::to_string(message.partition()) + "], at offset " + std::to_string(message.offset()));

}
}