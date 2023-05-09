#pragma once

#include <nox/renderer/format.h>

#include <string>
#include <vector>

namespace NOX {

struct BufferAccessMethod {
    enum {
        STATIC = (1 << 0),
        DYNAMIC = (1 << 1)
    };
};

struct VertexAttribute {
    std::string name;
    Format format;
};

struct VertexFormat {
    std::vector<VertexAttribute> attributes;
};

} // namespace NOX
