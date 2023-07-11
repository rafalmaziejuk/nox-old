#pragma once

#if defined(NOX_STATIC)
inline constexpr auto isStaticLinking = true;
#else
inline constexpr auto isStaticLinking = false;
#endif

#if defined(NOX_DEBUG)
inline constexpr auto isDebugConfiguration = true;
#else
inline constexpr auto isDebugConfiguration = false;
#endif

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

#define NOX_UNUSED(x) (void)(x)

#define BIT(x) (1 << x)

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
