#pragma once

namespace NOX::ConfigHelper {

enum class Linking {
    STATIC,
    DYNAMIC
};
static inline constexpr Linking getLinking() {
#if defined(NOX_SHARED)
    return Linking::DYNAMIC;
#else
    return Linking::STATIC;
#endif
}

enum class BuildConfiguration {
    DEBUG,
    RELEASE
};
static inline constexpr BuildConfiguration getBuildConfiguration() {
#if defined(NOX_DEBUG)
    return BuildConfiguration::DEBUG;
#else
    return BuildConfiguration::RELEASE;
#endif
}

} // namespace NOX::ConfigHelper
