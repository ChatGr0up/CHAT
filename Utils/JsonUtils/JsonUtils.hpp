#pragma once
#include <string>
#include "jsoncpp/json/json.h"

namespace CHAT::Utils::Json {
using JsonValue = ::Json::Value;
using CharReaderBuilder = ::Json::CharReaderBuilder;
using StreamWriter = ::Json::StreamWriter;
using StreamWriterBuilder = ::Json::StreamWriterBuilder;    
class JsonUtils {
public:
    // 从文件加载 JSON
    static JsonValue loadFromFile(const std::string& filePath);

    // 保存 JSON 到文件
    static void saveToFile(const std::string& filePath, const JsonValue& j);

    // 转换 JSON 为字符串
    static std::string toString(const JsonValue& j);

    // 从字符串解析 JSON
    static JsonValue fromString(const std::string& jsonStr);

    // 获取 JSON 中某个字段的值，如果没有则返回默认值
    template<typename T>
    static T get(const JsonValue& j, const std::string& key, const T& defaultValue);
};
}
