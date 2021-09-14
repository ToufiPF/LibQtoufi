#include "log/ostreamlogger.hpp"

namespace ltoufi {

const char* CONSOLE_COLOR_PREFIX[] = {
    "\x1B[0m",
    "\x1B[0m",
    "\x1B[33m",
    "\x1B[31m",
    "\x1B[97;41m",
};

const char* CONSOLE_RESET_COLOR = "\x1B[0m";

OStreamLogger::OStreamLogger(std::ostream& out) :
    Logger(),
    mOut(out),
    mPrintTimestamp(false),
    mUseConsoleColors(true),
    mUseShortLevelName(false) {
}

void OStreamLogger::printTimestamp(bool timestamp) { mPrintTimestamp = timestamp; }
bool OStreamLogger::isPrintingTimestamp() const { return mPrintTimestamp; }

void OStreamLogger::useConsoleColors(bool useColors) { mUseConsoleColors = useColors; }
bool OStreamLogger::isUsingConsoleColors() const { return mUseConsoleColors; }

void OStreamLogger::useShortLevelName(bool useShortName) { mUseShortLevelName = useShortName; }
bool OStreamLogger::isUsingShortLevelName() const { return mUseShortLevelName; }

void OStreamLogger::commit(const LogEntry &e) {
    if (mPrintTimestamp)
        mOut << e.timestamp.toString(Qt::DateFormat::ISODateWithMs).toStdString() << ' ';

    if (mUseConsoleColors)
        mOut << CONSOLE_COLOR_PREFIX[e.level];

    if (mUseShortLevelName)
        mOut << e.level.toChar();
    else
        mOut << e.level.toString();

    if (mUseConsoleColors)
        mOut << CONSOLE_RESET_COLOR;

    mOut << ": " << e.text.toStdString();
}

}
