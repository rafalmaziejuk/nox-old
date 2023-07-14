#pragma once

#include <algorithm>
#include <string>

namespace NOX::Utilities {

inline std::string toLower(std::string str) {
    const auto customToLower = [](uint8_t character) { return static_cast<uint8_t>(std::tolower(character)); };
    std::transform(str.begin(), str.end(), str.begin(), customToLower);
    return str;
}

} // namespace NOX::Utilities
