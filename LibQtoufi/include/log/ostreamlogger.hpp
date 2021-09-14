#pragma once

#include "libqtoufi_global.hpp"

#include <ostream>
#include <iostream>
#include "log/logger.hpp"

namespace ltoufi {

/**
 * @brief The OStreamLogger class is a simple Logger implementation that dumps every commited entry into the given std::ostream
 */
class LIBQTOUFI_EXPORT OStreamLogger final : Logger {

public:
    explicit OStreamLogger(std::ostream& out = std::cout);

    void printTimestamp(bool timestamp);
    bool isPrintingTimestamp() const;

    void useConsoleColors(bool useColors);
    bool isUsingConsoleColors() const;

    void useShortLevelName(bool useShortName);
    bool isUsingShortLevelName() const;

protected:
    void commit(const LogEntry &e) override;

private:
    std::ostream& mOut;
    bool mPrintTimestamp;
    bool mUseConsoleColors;
    bool mUseShortLevelName;
};

}
