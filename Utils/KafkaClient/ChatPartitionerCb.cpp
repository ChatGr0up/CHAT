#include "ChatPartitionerCb.hpp"
#include <functional>
#include <cstdlib>

namespace CHAT::Utils::KafkaDataSender {

int32_t ChatPartitionerCb::partitioner_cb(const RdKafka::Topic*,
                                          const std::string* key,
                                          int32_t partition_cnt,
                                          void*) {
    if (!key || key->empty()) {
        return rand() % partition_cnt;
    }

    std::hash<std::string> hasher;
    return hasher(*key) % partition_cnt;
}

}