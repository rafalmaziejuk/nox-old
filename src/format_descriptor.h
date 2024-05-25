#pragma once

#include <nox/export.h>
#include <nox/format.h>

#include <cstdint>

namespace nox {

enum class VertexAttributeType : uint8_t {
    NONE,
    UNSIGNED_BYTE,
    UNSIGNED_SHORT,
    UNSIGNED_INT,
    BYTE,
    SHORT,
    INT,
    HALF_FLOAT,
    FLOAT,
    MAX
};

struct ImageFormatDescriptor {
    bool isColor{false};
    bool isDepth{false};
    bool isStencil{false};
    bool isDepthStencil{false};
};

struct VertexAttributeFormatDescriptor {
    VertexAttributeType vertexAttributeType{VertexAttributeType::NONE};
    uint8_t componentsCount{0u};
    uint8_t stride{0u};
    bool isNormalized{false};
};

NOX_EXPORT [[nodiscard]] ImageFormatDescriptor getImageFormatDescriptor(ImageFormat format);
NOX_EXPORT [[nodiscard]] VertexAttributeFormatDescriptor getVertexAttributeFormatDescriptor(VertexAttributeFormat format);

} // namespace nox
