#pragma once

#include "libqtoufi_global.hpp"

#include <QTextStream>
#include "log/logentry.hpp"

namespace ltoufi {

class Logger;

class LIBQTOUFI_EXPORT LogStream final {
    friend class Logger;

private:
    LogStream(Logger& owner, LogLevel l);
    LogStream(const LogStream&) = delete;
    LogStream(LogStream&&) = delete;

public:
    ~LogStream();

    template<typename T>
    LogStream& operator<< (const T& msg) {
        mTs << msg;
        return *this;
    }

private:
    Logger& mOwner;
    LogEntry mEntry;
    QTextStream mTs;
};

}
