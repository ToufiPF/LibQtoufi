QT = core gui widgets network
TEMPLATE = lib
DEFINES += LIBQTOUFI_LIBRARY

CONFIG += c++17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release) {
    TARGET = libqtoufi-d
    TARGET_PATH = $$OUT_PWD/debug
}
else {
    TARGET = libqtoufi
    TARGET_PATH = $$OUT_PWD/release
}

# copy the compiled library to bin/ with the post_build_* script
# 1st argument is the destination (./bin/${OS})
# 2nd argument is the emplacement of the compiled library
windows {
    QMAKE_POST_LINK += bash $$shell_quote($$PWD/post_build_win.sh) $$shell_quote($$PWD/bin/win) $$shell_quote($$TARGET_PATH) ;
}
unix {
    QMAKE_POST_LINK += bash $$shell_quote($$PWD/post_build_unix.sh) $$shell_quote($$PWD/bin/unix) $$shell_quote($$TARGET_PATH) ;

    # "make install" to install libs in /usr/lib
    target.path = /usr/lib
    INSTALLS += target
}


INCLUDEPATH += include/

SOURCES += \
    src/log/logger.cpp \
    src/log/logstream.cpp \
    src/log/ostreamlogger.cpp \
    src/net/simpletcpserver.cpp \
    src/net/simpletcpsocket.cpp

HEADERS += \
    include/log/logentry.hpp \
    include/log/logger.hpp \
    include/log/logstream.hpp \
    include/log/ostreamlogger.hpp \
    include/net/simpletcpserver.hpp \
    include/net/simpletcpsocket.hpp \
    libqtoufi_global.hpp
