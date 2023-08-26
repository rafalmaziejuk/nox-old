#pragma once

namespace NOX {

struct Config {
#if defined(NOX_STATIC)
    static constexpr auto staticEnabled = true;
#else
    static constexpr auto staticEnabled = false;
#endif

#if defined(NOX_DEBUG)
    static constexpr auto debugEnabled = true;
#else
    static constexpr auto debugEnabled = false;
#endif

#if defined(__GNUC__)
    static constexpr auto gccEnabled = true;
#else
    static constexpr auto gccEnabled = false;
#endif
};

} // namespace NOX
