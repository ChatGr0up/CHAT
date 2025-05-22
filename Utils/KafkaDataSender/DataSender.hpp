#pragma once

#include <string>
#include <librdkafka/rdkafkacpp.h>
#include <memory>

namespace CHAT::Utils::KafkaDataSender {
class DataSender {
public:
    explicit DataSender(const std::string& topic);

    ~DataSender();

    bool open();

    void close();

    void sendFile(const std::string& key, const std::string& filePath);

    void sendMessage(const std::string& key, const std::string& message);
private:
    std::unique_ptr<RdKafka::Producer> producer;


}    
}