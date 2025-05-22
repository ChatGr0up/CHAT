#include "ChatEventCb.hpp"
#include <librdkafka/rdkafkacpp.h>
#include "Logger/LogMacroDef.hpp"

namespace CHAT::Utils::KafkaDataSender {

void ChatEventCb::event_cb(RdKafka::Event& event)
{
    switch (event.type()) {
        case RdKafka::Event::EVENT_ERROR:
            ERROR("ChatEventCb::event_cb", "Kafka error: " + event.str());
            if (event.err() == RdKafka::ERR__ALL_BROKERS_DOWN) {
                ERROR("ChatEventCb::event_cb", "All Kafka brokers are down!");
            }
            break;

        case RdKafka::Event::EVENT_STATS:
            TRACE("ChatEventCb::event_cb", "Kafka stats: " + event.str());
            break;

        case RdKafka::Event::EVENT_LOG:
            TRACE("ChatEventCb::event_cb", "Kafka log: severity=" +
                  std::to_string(event.severity()) + " facility=" + event.fac() +
                  " message=" + event.str());
            break;

        case RdKafka::Event::EVENT_THROTTLE:
            TRACE("ChatEventCb::event_cb", "Kafka throttled: broker=" + event.broker_name() +
                  " throttle_time=" + std::to_string(event.throttle_time()) + "ms");
            break;

        default:
            TRACE("ChatEventCb::event_cb", "Kafka unknown event: type=" +
                  std::to_string(event.type()) + " message=" + event.str());
            break;
    }
}

}