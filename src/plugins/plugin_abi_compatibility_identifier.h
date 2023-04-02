#pragma once

#include <nox/version.h>

#define BUILD_TYPE_BIT 0
#define COMPILER_BIT 1
#define API_VERSION_BIT 3

#if defined(NOX_DEBUG)
#define BUILD_TYPE_VALUE 0
#else
#define BUILD_TYPE_VALUE 1
#endif

#if defined(_MSC_VER)
#define COMPILER_VALUE 0
#elif defined(__GNUC__)
#define COMPILER_VALUE 1
#elif defined(__clang__)
#define COMPILER_VALUE 2
#endif

#define NOX_ABI_COMPATIBILITY_IDENTIFIER   \
    (BUILD_TYPE_VALUE << BUILD_TYPE_BIT) + \
        (COMPILER_VALUE << COMPILER_BIT) + \
        (NOX_VERSION << API_VERSION_BIT)
