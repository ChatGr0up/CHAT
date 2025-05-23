#include <librdkafka/rdkafkacpp.h>
#include "Logger/LogMacroDef.hpp"
#include <sstream>

namespace CHAT::Utils::KafkaDataSender {
class ChatRebalanceCb : public RdKafka::RebalanceCb {
public:
    void rebalance_cb(RdKafka::KafkaConsumer *consumer,
                      RdKafka::ErrorCode err,
                      std::vector<RdKafka::TopicPartition *> &partitions) override;
                      
    ~ChatRebalanceCb() override = default;
};
}