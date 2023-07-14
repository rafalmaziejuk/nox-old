#pragma once

#include "core/core.h"
#include "core/logger.h"

#if defined(NOX_DEBUG)
#define NOX_ASSERT_MSG(expression, message, ...)              \
    if (expression) {                                         \
        NOX_LOG_ERROR(ASSERT, "({}) {}", __LINE__, __FILE__); \
        NOX_LOG_ERROR(ASSERT, message, ##__VA_ARGS__);        \
        NOX_DEBUG_BREAK();                                    \
    }

#define NOX_ASSERT(expression)                                \
    if (expression) {                                         \
        NOX_LOG_ERROR(ASSERT, "({}) {}", __LINE__, __FILE__); \
        NOX_LOG_ERROR(ASSERT, "{}", #expression);             \
        NOX_DEBUG_BREAK();                                    \
    }
#else
#define NOX_ASSERT(expression) NOX_UNUSED(expression)
#define NOX_ASSERT_MSG(expression, ...) NOX_UNUSED(expression)
#endif
