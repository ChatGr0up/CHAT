#include "JsonUtils.hpp"
#include <fstream>
#include <stdexcept>

namespace CHAT::Utils::Json {
JsonValue JsonUtils::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    JsonValue j;
    file >> j;
    return j;
}

void JsonUtils::saveToFile(const std::string& filePath, const JsonValue& j) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    file << j.dump(4); // 格式化为4个空格
}

std::string JsonUtils::toString(const JsonValue& j) {
    return j.dump(4); // 格式化为4个空格
}

JsonValue JsonUtils::fromString(const std::string& jsonStr) {
    return JsonValue::parse(jsonStr);
}

template<typename T>
T JsonUtils::get(const JsonValue& j, const std::string& key, const T& defaultValue) {
    if (j.contains(key)) {
        return j[key].get<T>();
    }
    return defaultValue;
}
}
