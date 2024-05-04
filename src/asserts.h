#pragma once

// clang-format off

#if defined(NOX_DISABLE_ASSERTS) || !defined(NOX_DEBUG)
    #define NOX_ASSERT(condition) (void)(condition)
#else
    #include <cassert>
    #define NOX_ASSERT(condition) assert(condition)
#endif

#define NOX_ASSERT_MSG(condition, message) NOX_ASSERT((condition) && (message))

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
