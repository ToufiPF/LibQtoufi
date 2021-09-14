#pragma once

#include "libqtoufi_global.hpp"

#include <QString>
#include <QDateTime>
#include <string>
#include <cstdint>

namespace ltoufi {

class LIBQTOUFI_EXPORT LogLevel final {
public:
    enum Value : uint8_t {
        DEBUG, INFO, WARNING, ERROR, FATAL,
        _COUNT_
    };

public:
    LogLevel() = delete;

    /**
     * @brief LogLevel creates a LogLevel. ```LogLevel l = LogLevel::INFO;```
     * @param level
     */
    constexpr LogLevel(Value level) : mValue(level) {}

    /**
     * @brief operator Value auto unwrapp to use in switch/comparisons
     */
    constexpr operator Value() const { return mValue; }

    // don't allow use in if()
    explicit operator bool() = delete;

    /**
     * @brief toString returns a string description of the LogLevel
     * @return std::string
     */
    constexpr const char* toString() const {
        switch (mValue) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        case FATAL: return "FATAL";
        default: return "UNKNOWN";
        }
    }

    /**
     * @brief toChar returns the char corresponding to the LogLevel
     * @return char
     */
    constexpr char toChar() const {
        switch (mValue) {
        case DEBUG: return 'D';
        case INFO: return 'I';
        case WARNING: return 'W';
        case ERROR: return 'E';
        case FATAL: return 'F';
        default: return 'U';
        }
    }

private:
    Value mValue;
};

struct LIBQTOUFI_EXPORT LogEntry {
    QDateTime timestamp;
    QString text;
    LogLevel level;
};

}
