#pragma once

#include "utilities/logger.h"

#if defined(NOX_DEBUG)
#if defined(NOX_WIN32)
#define NOX_DEBUG_BREAK() __debugbreak()
#elif defined(NOX_UNIX)
#include <signal.h>
#define NOX_DEBUG_BREAK() raise(SIGTRAP)
#endif

#define NOX_ASSERT_MSG(expression, message, ...)              \
    if (expression) {                                         \
        NOX_LOG_ERROR(ASSERT, "({}) {}", __LINE__, __FILE__); \
        NOX_LOG_ERROR(ASSERT, message, __VA_ARGS__);          \
        NOX_DEBUG_BREAK();                                    \
    }

#define NOX_ASSERT(expression)                                \
    if (expression) {                                         \
        NOX_LOG_ERROR(ASSERT, "({}) {}", __LINE__, __FILE__); \
        NOX_LOG_ERROR(ASSERT, "{}", #expression);             \
        NOX_DEBUG_BREAK();                                    \
    }
#else
#define NOX_ASSERT(expression) (void)(expression)
#define NOX_ASSERT_MSG(expression, ...) NOX_ASSERT(expression)
#endif
