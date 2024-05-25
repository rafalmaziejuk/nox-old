#pragma once

namespace nox {

inline bool assertDisabled = false;

[[nodiscard]] inline bool &getAssertDisabled() {
    return assertDisabled;
}

} // namespace nox

// clang-format off

#if defined(NOX_DISABLE_ASSERTS) || !defined(NOX_DEBUG)
    #define NOX_ASSERT(condition) (void)(condition)
#else
    #include <cassert>
    #define NOX_ASSERT(condition) assert((condition) || (assertDisabled))
#endif

#define NOX_ASSERT_MSG(condition, message) NOX_ASSERT((condition) && (message))

#define NOX_ENSURE_RETURN_FALSE(condition)          \
    if (const auto result = (condition); !result) { \
        NOX_ASSERT(result);                         \
        return false;                               \
    }

#define NOX_ENSURE_RETURN_FALSE_MSG(condition, message) \
    if (const auto result = (condition); !result) {     \
        NOX_ASSERT_MSG(result, message);                \
        return false;                                   \
    }

#define NOX_ENSURE_RETURN_NULLPTR(condition)        \
    if (const auto result = (condition); !result) { \
        NOX_ASSERT(result);                         \
        return nullptr;                             \
    }

#define NOX_ENSURE_RETURN_NULLPTR_MSG(condition, message) \
    if (const auto result = (condition); !result) {       \
        NOX_ASSERT_MSG(result, message);                  \
        return nullptr;                                   \
    }

// clang-format on
