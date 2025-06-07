#pragma once

#include <string>

namespace CHAT::Utils::CommonUtils {
class RandomCode {
public:
    static std::string generateVerifyCodeStr(const std::string& sourceStr);

    static int generateVerifyCodeInterger(const std::string& sourceStr);
};
}