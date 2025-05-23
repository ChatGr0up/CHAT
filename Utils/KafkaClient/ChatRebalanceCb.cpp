#include <librdkafka/rdkafkacpp.h>
#include "Logger/LogMacroDef.hpp"
#include "ChatRebalanceCb.hpp"
#include <sstream>

namespace CHAT::Utils::KafkaDataSender {
void ChatRebalanceCb::rebalance_cb(RdKafka::KafkaConsumer *consumer, RdKafka::ErrorCode err,
    std::vector<RdKafka::TopicPartition *> &partitions) {
        std::ostringstream oss;
        for (const auto &p : partitions) {
            oss << "[" << p->topic() << ":" << p->partition() << "] ";
        }

        switch (err) {
            case RdKafka::ERR__ASSIGN_PARTITIONS:
                TRACE("KafkaRebalance", "Partitions assigned: " + oss.str());

                // 可插入自定义的 assignment 策略，如限制某些分区不被消费
                // 或统计负载分布等

                consumer->assign(partitions);
                break;

            case RdKafka::ERR__REVOKE_PARTITIONS:
                TRACE("KafkaRebalance", "Partitions revoked: " + oss.str());

                // 可在此处保存 offset、统计 metrics、释放资源等
                consumer->unassign();
                break;

            default:
                ERROR("KafkaRebalance", "Unexpected rebalance error: " + RdKafka::err2str(err));
                consumer->unassign();
                break;
        }
    }
};// namespace CHAT::Utils::KafkaDataSender