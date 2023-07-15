#pragma once

#include <cstdint>

namespace NOX {

struct ShaderStage {
    enum {
        VERTEX = (1 << 0),
        FRAGMENT = (1 << 1)
    };
};

} // namespace NOX
