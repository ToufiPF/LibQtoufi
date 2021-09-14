#pragma once

#include <QtCore/qglobal.h>

#if defined(LIBQTOUFI_LIBRARY)
#  define LIBQTOUFI_EXPORT Q_DECL_EXPORT
#else
#  define LIBQTOUFI_EXPORT Q_DECL_IMPORT
#endif
