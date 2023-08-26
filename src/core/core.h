#pragma once

#include "core/logger.h"

#define NOX_UNUSED(x) (void)(x)

#define BIT(x) (1 << x)

#if defined(NOX_DEBUG)
#if defined(NOX_WIN32)
#define NOX_DEBUG_BREAK() __debugbreak()
#elif defined(NOX_UNIX)
#include <signal.h>
#define NOX_DEBUG_BREAK() raise(SIGTRAP)
#endif
#else
#define NOX_DEBUG_BREAK()
#endif

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

#define NOX_CASE_RETURN_VALUE(label, value) \
    case label:                             \
        return value

#define NOX_CASE_RETURN_STRING(label, value) \
    case label:                              \
        return #value

template <typename TargetType, typename SourceType>
const TargetType *downcast(const SourceType &object) {
    const auto *targetObject = dynamic_cast<const TargetType *>(&object);
    if (targetObject == nullptr) {
        NOX_DEBUG_BREAK();
    }
    return targetObject;
}
