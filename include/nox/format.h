#pragma once

#include <vector>

namespace nox {

enum class ImageFormat {
    R8UI,
    R16UI,
    R32UI,
    RG8UI,
    RG16UI,
    RG32UI,
    RGB8UI,
    RGB16UI,
    RGB32UI,
    RGBA8UI,
    RGBA16UI,
    RGBA32UI,
    R8I,
    R16I,
    R32I,
    RG8I,
    RG16I,
    RG32I,
    RGB8I,
    RGB16I,
    RGB32I,
    RGBA8I,
    RGBA16I,
    RGBA32I,
    R8_UNORM,
    R16_UNORM,
    R32_UNORM,
    RG8_UNORM,
    RG16_UNORM,
    RG32_UNORM,
    RGB8_UNORM,
    RGB16_UNORM,
    RGB32_UNORM,
    RGBA8_UNORM,
    RGBA16_UNORM,
    RGBA32_UNORM,
    R8_SNORM,
    R16_SNORM,
    R32_SNORM,
    RG8_SNORM,
    RG16_SNORM,
    RG32_SNORM,
    RGB8_SNORM,
    RGB16_SNORM,
    RGB32_SNORM,
    RGBA8_SNORM,
    RGBA16_SNORM,
    RGBA32_SNORM,
    R16F,
    R32F,
    RG16F,
    RG32F,
    RGB16F,
    RGB32F,
    RGBA16F,
    RGBA32F,
    DEPTH16,
    DEPTH24,
    DEPTH32,
    DEPTH32F,
    STENCIL8,
    DEPTH24_STENCIL8,
    DEPTH32F_STENCIL8,
    MAX
};

enum class VertexAttributeFormat {
    R8UI,
    R16UI,
    R32UI,
    RG8UI,
    RG16UI,
    RG32UI,
    RGB8UI,
    RGB16UI,
    RGB32UI,
    RGBA8UI,
    RGBA16UI,
    RGBA32UI,
    R8I,
    R16I,
    R32I,
    RG8I,
    RG16I,
    RG32I,
    RGB8I,
    RGB16I,
    RGB32I,
    RGBA8I,
    RGBA16I,
    RGBA32I,
    R8_UNORM,
    R16_UNORM,
    R32_UNORM,
    RG8_UNORM,
    RG16_UNORM,
    RG32_UNORM,
    RGB8_UNORM,
    RGB16_UNORM,
    RGB32_UNORM,
    RGBA8_UNORM,
    RGBA16_UNORM,
    RGBA32_UNORM,
    R8_SNORM,
    R16_SNORM,
    R32_SNORM,
    RG8_SNORM,
    RG16_SNORM,
    RG32_SNORM,
    RGB8_SNORM,
    RGB16_SNORM,
    RGB32_SNORM,
    RGBA8_SNORM,
    RGBA16_SNORM,
    RGBA32_SNORM,
    R16F,
    R32F,
    RG16F,
    RG32F,
    RGB16F,
    RGB32F,
    RGBA16F,
    RGBA32F,
    MAX
};

enum class UniformDataFormat {
    NONE,
    FLOAT1
};

} // namespace nox
