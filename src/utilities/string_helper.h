#pragma once

#include <algorithm>
#include <string>

namespace NOX::StringHelper {

inline void toLower(std::string &value) {
    const auto customToLower = [](uint8_t c) { return static_cast<uint8_t>(std::tolower(c)); };
    std::transform(value.begin(), value.end(), value.begin(), customToLower);
}

} // namespace NOX::StringHelper
