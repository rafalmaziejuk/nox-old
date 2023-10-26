#pragma once

#include <algorithm>
#include <string>

namespace NOX::Utilities {

inline std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](uint8_t c) { return static_cast<uint8_t>(std::tolower(c)); });
    return str;
}

} // namespace NOX::Utilities
