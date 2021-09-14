#include "log/logger.hpp"

namespace ltoufi {

LogStream Logger::log(LogLevel level) {
    return LogStream(*this, level);
}

};
