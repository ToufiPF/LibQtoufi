#include "log/logstream.hpp"
#include "log/logger.hpp"

namespace ltoufi {

LogStream::LogStream(Logger& owner, LogLevel l) :
    mOwner(owner),
    mEntry{.timestamp = QDateTime::currentDateTime(), .text = QString(), .level = l },
    mTs() {
}

LogStream::~LogStream() {
    mOwner.commit(mEntry);
}

};
