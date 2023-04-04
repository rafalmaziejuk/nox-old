#pragma once

#include "utilities/logger.h"

// clang-format off
#if !defined(NOX_DEBUG_BREAK) && defined(NOX_DEBUG)
    #if defined(NOX_WIN32)
        #define NOX_DEBUG_BREAK() __debugbreak()
    #elif defined(NOX_UNIX)
        #include <signal.h>
        #define NOX_DEBUG_BREAK() raise(SIGTRAP)
    #endif
#else
    #define NOX_DEBUG_BREAK() (void)0
#endif

#if !defined(NOX_ASSERT_IF) && defined(NOX_DEBUG)
    #define NOX_ASSERT_IF(expression, ...)                                 \
        if (expression) {                                                  \
            NOX::DebugHelper::debugBreak(__LINE__, __FILE__, __VA_ARGS__); \
            NOX_DEBUG_BREAK();                                             \
        }
#else
    #define NOX_ASSERT_IF(expression, ...) (void)(expression)
#endif

#if !defined(NOX_ABORT_IF) && defined(NOX_DEBUG)
    #define NOX_ABORT_IF(expression, ...)                                          \
        if (expression) {                                                          \
            NOX::DebugHelper::abortUnrecoverable(__LINE__, __FILE__, __VA_ARGS__); \
            abort();                                                               \
        }
#else
    #define NOX_ABORT_IF(expression, ...) abort();
#endif
// clang-format on

namespace NOX::DebugHelper {

template <typename... Args>
static void debugBreak(const int32_t line, const char *file, Args &&...args) {
    NOX_LOG(DEBUG, ERROR, "Assert {0} ({1}):", file, line);
    NOX_LOG(DEBUG, ERROR, std::forward<Args>(args)...);
}

template <typename... Args>
static void debugBreak(const int32_t line, const char *file) {
    NOX_LOG(DEBUG, ERROR, "Assert {0} ({1})", file, line);
}

template <typename... Args>
static void abortUnrecoverable(const int32_t line, const char *file, Args &&...args) {
    NOX_LOG(DEBUG, CRITICAL, "Abort {0} ({1}):", file, line, std::forward<Args>(args)...);
    NOX_LOG(DEBUG, CRITICAL, std::forward<Args>(args)...);
}

template <typename... Args>
static void abortUnrecoverable(const int32_t line, const char *file) {
    NOX_LOG(DEBUG, CRITICAL, "Abort {0} ({1})", file, line);
}

} // namespace NOX::DebugHelper
