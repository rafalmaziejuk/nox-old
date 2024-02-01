#pragma once

namespace nox {

inline bool debugBreakEnabled = true;

void setDebugBreakEnabled(bool value);
[[nodiscard]] bool isDebugBreakEnabled();

} // namespace nox

// clang-format off

#if defined(NOX_WINDOWS)
    #define NOX_DEBUG_BREAK() __debugbreak()
#elif defined(NOX_LINUX)
    #include <signal.h>
    #define NOX_DEBUG_BREAK() raise(SIGTRAP)
#endif

#if defined(NOX_DISABLE_ASSERTS) || !defined(NOX_DEBUG)
    #define NOX_ASSERT(condition) (void)(0)
#else
    #define NOX_ASSERT(condition)                      \
        if (!(condition) && (isDebugBreakEnabled())) { \
            NOX_DEBUG_BREAK();                         \
        }
#endif

#define NOX_ASSERT_MSG(condition, message) NOX_ASSERT(condition)

#define NOX_ENSURE_RETURN_FALSE(condition) \
    if (!(condition)) {                    \
        NOX_ASSERT(condition);             \
        return false;                      \
    }

#define NOX_ENSURE_RETURN_FALSE_MSG(condition, message) \
    if (!(condition)) {                                 \
        NOX_ASSERT_MSG(condition, message);             \
        return false;                                   \
    }

#define NOX_ENSURE_RETURN_NULLPTR(condition) \
    if (!(condition)) {                      \
        NOX_ASSERT(condition);               \
        return nullptr;                      \
    }

#define NOX_ENSURE_RETURN_NULLPTR_MSG(condition, message) \
    if (!(condition)) {                                   \
        NOX_ASSERT_MSG(condition, message);               \
        return nullptr;                                   \
    }

// clang-format on
