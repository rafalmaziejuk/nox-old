#pragma once

#include <algorithm>
#include <string>

namespace NOX::Utilities {

inline std::string toLower(std::string value) {
    const auto customToLower = [](uint8_t character) { return static_cast<uint8_t>(std::tolower(character)); };
    std::transform(value.begin(), value.end(), value.begin(), customToLower);
    return value;
}

enum class Linking {
    STATIC,
    DYNAMIC
};
constexpr Linking getLinking() {
#if defined(NOX_STATIC)
    return Linking::STATIC;
#else
    return Linking::DYNAMIC;
#endif
}

enum class BuildConfiguration {
    DEBUG,
    RELEASE
};
constexpr BuildConfiguration getBuildConfiguration() {
#if defined(NOX_DEBUG)
    return BuildConfiguration::DEBUG;
#else
    return BuildConfiguration::RELEASE;
#endif
}

} // namespace NOX::Utilities
