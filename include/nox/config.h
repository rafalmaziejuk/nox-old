#pragma once

// clang-format off

#if defined(NOX_DISABLE_ASSERT) || !defined(NOX_DEBUG)
    #define NOX_ASSERT(condition) (void)(0)
#else
    #include <cassert>
    #define NOX_ASSERT(condition) assert(condition)
#endif

#define NOX_ASSERT_MSG(condition, message) NOX_ASSERT((condition) && (message))

// clang-format on
