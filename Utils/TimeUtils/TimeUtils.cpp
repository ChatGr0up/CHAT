#include "TimeUtils.hpp"
#include <chrono>

namespace CHAT::Utils::TimeUtils{
uint32_t TimeUtils::getCurrentUtcSeconds()
{
    return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
}
}