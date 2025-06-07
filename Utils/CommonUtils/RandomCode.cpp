#include "RandomCode.hpp"
#include <random>

namespace CHAT::Utils::CommonUtils {
std::string RandomCode::generateVerifyCodeStr(const std::string& sourceStr)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    return std::to_string(dis(gen));
}
int RandomCode::generateVerifyCodeInterger(const std::string &sourceStr)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    return dis(gen);
}
}