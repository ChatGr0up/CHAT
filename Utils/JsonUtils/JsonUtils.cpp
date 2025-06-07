#include "JsonUtils.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>

namespace CHAT::Utils::Json {

JsonValue JsonUtils::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ifstream::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    JsonValue root;
    CharReaderBuilder readerBuilder;
    std::string errors;

    bool parsingSuccessful = ::Json::parseFromStream(readerBuilder, file, &root, &errors);
    if (!parsingSuccessful) {
        throw std::runtime_error("Failed to parse JSON from file: " + errors);
    }

    return root;
}

void JsonUtils::saveToFile(const std::string& filePath, const JsonValue& j) {
    std::ofstream file(filePath, std::ofstream::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    StreamWriterBuilder writerBuilder;
    std::unique_ptr<StreamWriter> writer(writerBuilder.newStreamWriter());
    writer->write(j, &file);
}

std::string JsonUtils::toString(const JsonValue& j) {
    StreamWriterBuilder writerBuilder;
    return ::Json::writeString(writerBuilder, j);
}

JsonValue JsonUtils::fromString(const std::string& jsonStr) {
    JsonValue root;
    if (jsonStr.empty()) {
        return root;
    }
    CharReaderBuilder readerBuilder;
    std::istringstream stream(jsonStr);
    std::string errors;

    bool parsingSuccessful = ::Json::parseFromStream(readerBuilder, stream, &root, &errors);
    if (!parsingSuccessful) {
        throw std::runtime_error("Failed to parse JSON from string: " + errors);
    }

    return root;
}

template<typename T>
T JsonUtils::get(const JsonValue& j, const std::string& key, const T& defaultValue) {
    if (j.isMember(key)) {
        return j[key].as<T>();
    }
    return defaultValue;
}
}
