#pragma once

#include "libqtoufi_global.hpp"

#include "log/logentry.hpp"
#include "log/logstream.hpp"

namespace ltoufi {

class LIBQTOUFI_EXPORT Logger {
    friend class LogStream;

public:
    explicit Logger() = default;

    /**
     * @brief log creates a LogStream with the given LogLevel.
     * @param level the LogLevel that the message will have
     * @return LogStream with the given level.
     */
    LogStream log(LogLevel level);

    /**
     * @brief debug creates a LogStream with level LogLevel::DEBUG
     * @return
     */
    inline LogStream debug() { return log(LogLevel::DEBUG); }

    /**
     * @brief info creates a LogStream with level LogLevel::INFO
     * @return
     */
    inline LogStream info() { return log(LogLevel::INFO); }

    /**
     * @brief warning creates a LogStream with level LogLevel::WARNING
     * @return
     */
    inline LogStream warning() { return log(LogLevel::WARNING); }

    /**
     * @brief error creates a LogStream with level LogLevel::ERROR
     * @return
     */
    inline LogStream error() { return log(LogLevel::ERROR); }

    /**
     * @brief fatal creates a LogStream with level LogLevel::FATAL
     * @return
     */
    inline LogStream fatal() { return log(LogLevel::FATAL); }

protected:
    virtual void commit(const LogEntry& e) = 0;
};

}
