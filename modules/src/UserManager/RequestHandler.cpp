#include "RequestHandler.hpp"

namespace CHAT::Module {
uint32_t RequestHandler::loopInterval()
{
    return 0;
} 
    
uint32_t RequestHandler::loopDelayTime()
{
    return 0;
} 

bool RequestHandler::svc()
{
    return true;
} 

JsonValue RequestHandler::handleRequestAsync(const JsonValue &request)
{
    return JsonValue();
}

JsonValue RequestHandler::handleRequestSync(const JsonValue &request)
{
    return JsonValue();
}
}