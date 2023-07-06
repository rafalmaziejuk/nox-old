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

#define NOX_UNUSED(x) (void)(x)

#define BIT(x) (1 << x)
