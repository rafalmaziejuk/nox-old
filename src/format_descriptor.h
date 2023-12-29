#pragma once

#include <nox/format.h>

#include <array>
#include <cstdint>

namespace nox {

struct ImageFormatDescriptor {
    bool isColor{false};
    bool isDepth{false};
    bool isStencil{false};
    bool isDepthStencil{false};
};

using ImageFormatDescriptors = std::array<ImageFormatDescriptor, static_cast<size_t>(ImageFormat::MAX)>;

// clang-format off
inline constexpr ImageFormatDescriptors imageFormatDescriptors{{
    {true , false, false, false}, // R8UI
    {true , false, false, false}, // R16UI
    {true , false, false, false}, // R32UI
    {true , false, false, false}, // RG8UI
    {true , false, false, false}, // RG16UI
    {true , false, false, false}, // RG32UI
    {true , false, false, false}, // RGB8UI
    {true , false, false, false}, // RGB16UI
    {true , false, false, false}, // RGB32UI
    {true , false, false, false}, // RGBA8UI
    {true , false, false, false}, // RGBA16UI
    {true , false, false, false}, // RGBA32UI
    {true , false, false, false}, // R8I
    {true , false, false, false}, // R16I
    {true , false, false, false}, // R32I
    {true , false, false, false}, // RG8I
    {true , false, false, false}, // RG16I
    {true , false, false, false}, // RG32I
    {true , false, false, false}, // RGB8I
    {true , false, false, false}, // RGB16I
    {true , false, false, false}, // RGB32I
    {true , false, false, false}, // RGBA8I
    {true , false, false, false}, // RGBA16I
    {true , false, false, false}, // RGBA32I
    {true , false, false, false}, // R8_UNORM
    {true , false, false, false}, // R16_UNORM
    {true , false, false, false}, // R32_UNORM
    {true , false, false, false}, // RG8_UNORM
    {true , false, false, false}, // RG16_UNORM
    {true , false, false, false}, // RG32_UNORM
    {true , false, false, false}, // RGB8_UNORM
    {true , false, false, false}, // RGB16_UNORM
    {true , false, false, false}, // RGB32_UNORM
    {true , false, false, false}, // RGBA8_UNORM
    {true , false, false, false}, // RGBA16_UNORM
    {true , false, false, false}, // RGBA32_UNORM
    {true , false, false, false}, // R8_SNORM
    {true , false, false, false}, // R16_SNORM
    {true , false, false, false}, // R32_SNORM
    {true , false, false, false}, // RG8_SNORM
    {true , false, false, false}, // RG16_SNORM
    {true , false, false, false}, // RG32_SNORM
    {true , false, false, false}, // RGB8_SNORM
    {true , false, false, false}, // RGB16_SNORM
    {true , false, false, false}, // RGB32_SNORM
    {true , false, false, false}, // RGBA8_SNORM
    {true , false, false, false}, // RGBA16_SNORM
    {true , false, false, false}, // RGBA32_SNORM
    {true , false, false, false}, // R16F
    {true , false, false, false}, // R32F
    {true , false, false, false}, // RG16F
    {true , false, false, false}, // RG32F
    {true , false, false, false}, // RGB16F
    {true , false, false, false}, // RGB32F
    {true , false, false, false}, // RGBA16F
    {true , false, false, false}, // RGBA32F
    {false, true , false, false}, // DEPTH16
    {false, true , false, false}, // DEPTH24
    {false, true , false, false}, // DEPTH32
    {false, true , false, false}, // DEPTH32F
    {false, false, true , false}, // STENCIL8
    {false, false, false, true }, // DEPTH24_STENCIL8
    {false, false, false, true }  // DEPTH32F_STENCIL8
}};
// clang-format on

[[nodiscard]] inline ImageFormatDescriptor getImageFormatDescriptor(ImageFormat format) {
    auto index = static_cast<size_t>(format);
    return imageFormatDescriptors.at(index);
}

enum class VertexAttributeDataType : uint8_t {
    UNSIGNED_BYTE,
    UNSIGNED_SHORT,
    UNSIGNED_INT,
    BYTE,
    SHORT,
    INT,
    HALF_FLOAT,
    FLOAT
};

struct VertexAttributeFormatDescriptor {
    VertexAttributeDataType vertexAttributeDataType;
    uint8_t componentsCount{0u};
    uint8_t stride{0u};
    bool isNormalized{false};
};

using VertexAttributeFormatDescriptors = std::array<VertexAttributeFormatDescriptor, static_cast<size_t>(VertexAttributeFormat::MAX)>;

// clang-format off
inline constexpr VertexAttributeFormatDescriptors vertexAttributeFormatDescriptors{{
    {VertexAttributeDataType::UNSIGNED_BYTE , 1u, 1u * 1u, false}, // R8UI
    {VertexAttributeDataType::UNSIGNED_SHORT, 1u, 1u * 2u, false}, // R16UI
    {VertexAttributeDataType::UNSIGNED_INT  , 1u, 1u * 4u, false}, // R32UI
    {VertexAttributeDataType::UNSIGNED_BYTE , 2u, 2u * 1u, false}, // RG8UI
    {VertexAttributeDataType::UNSIGNED_SHORT, 2u, 2u * 2u, false}, // RG16UI
    {VertexAttributeDataType::UNSIGNED_INT  , 2u, 2u * 4u, false}, // RG32UI
    {VertexAttributeDataType::UNSIGNED_BYTE , 3u, 3u * 1u, false}, // RGB8UI
    {VertexAttributeDataType::UNSIGNED_SHORT, 3u, 3u * 2u, false}, // RGB16UI
    {VertexAttributeDataType::UNSIGNED_INT  , 3u, 3u * 4u, false}, // RGB32UI
    {VertexAttributeDataType::UNSIGNED_BYTE , 4u, 4u * 1u, false}, // RGBA8UI
    {VertexAttributeDataType::UNSIGNED_SHORT, 4u, 4u * 2u, false}, // RGBA16UI
    {VertexAttributeDataType::UNSIGNED_INT  , 4u, 4u * 4u, false}, // RGBA32UI
    {VertexAttributeDataType::BYTE          , 1u, 1u * 1u, false}, // R8I
    {VertexAttributeDataType::SHORT         , 1u, 1u * 2u, false}, // R16I
    {VertexAttributeDataType::INT           , 1u, 1u * 4u, false}, // R32I
    {VertexAttributeDataType::BYTE          , 2u, 2u * 1u, false}, // RG8I
    {VertexAttributeDataType::SHORT         , 2u, 2u * 2u, false}, // RG16I
    {VertexAttributeDataType::INT           , 2u, 2u * 4u, false}, // RG32I
    {VertexAttributeDataType::BYTE          , 3u, 3u * 1u, false}, // RGB8I
    {VertexAttributeDataType::SHORT         , 3u, 3u * 2u, false}, // RGB16I
    {VertexAttributeDataType::INT           , 3u, 3u * 4u, false}, // RGB32I
    {VertexAttributeDataType::BYTE          , 4u, 4u * 1u, false}, // RGBA8I
    {VertexAttributeDataType::SHORT         , 4u, 4u * 2u, false}, // RGBA16I
    {VertexAttributeDataType::INT           , 4u, 4u * 4u, false}, // RGBA32I
    {VertexAttributeDataType::UNSIGNED_BYTE , 1u, 1u * 1u, true }, // R8_UNORM
    {VertexAttributeDataType::UNSIGNED_SHORT, 1u, 1u * 2u, true }, // R16_UNORM
    {VertexAttributeDataType::UNSIGNED_INT  , 1u, 1u * 4u, true }, // R32_UNORM
    {VertexAttributeDataType::UNSIGNED_BYTE , 2u, 2u * 1u, true }, // RG8_UNORM
    {VertexAttributeDataType::UNSIGNED_SHORT, 2u, 2u * 2u, true }, // RG16_UNORM
    {VertexAttributeDataType::UNSIGNED_INT  , 2u, 2u * 4u, true }, // RG32_UNORM
    {VertexAttributeDataType::UNSIGNED_BYTE , 3u, 3u * 1u, true }, // RGB8_UNORM
    {VertexAttributeDataType::UNSIGNED_SHORT, 3u, 3u * 2u, true }, // RGB16_UNORM
    {VertexAttributeDataType::UNSIGNED_INT  , 3u, 3u * 4u, true }, // RGB32_UNORM
    {VertexAttributeDataType::UNSIGNED_BYTE , 4u, 4u * 1u, true }, // RGBA8_UNORM
    {VertexAttributeDataType::UNSIGNED_SHORT, 4u, 4u * 2u, true }, // RGBA16_UNORM
    {VertexAttributeDataType::UNSIGNED_INT  , 4u, 4u * 4u, true }, // RGBA32_UNORM
    {VertexAttributeDataType::BYTE          , 1u, 1u * 1u, true }, // R8_SNORM
    {VertexAttributeDataType::SHORT         , 1u, 1u * 2u, true }, // R16_SNORM
    {VertexAttributeDataType::INT           , 1u, 1u * 4u, true }, // R32_SNORM
    {VertexAttributeDataType::BYTE          , 2u, 2u * 1u, true }, // RG8_SNORM
    {VertexAttributeDataType::SHORT         , 2u, 2u * 2u, true }, // RG16_SNORM
    {VertexAttributeDataType::INT           , 2u, 2u * 4u, true }, // RG32_SNORM
    {VertexAttributeDataType::BYTE          , 3u, 3u * 1u, true }, // RGB8_SNORM
    {VertexAttributeDataType::SHORT         , 3u, 3u * 2u, true }, // RGB16_SNORM
    {VertexAttributeDataType::INT           , 3u, 3u * 4u, true }, // RGB32_SNORM
    {VertexAttributeDataType::BYTE          , 4u, 4u * 1u, true }, // RGBA8_SNORM
    {VertexAttributeDataType::SHORT         , 4u, 4u * 2u, true }, // RGBA16_SNORM
    {VertexAttributeDataType::INT           , 4u, 4u * 4u, true }, // RGBA32_SNORM
    {VertexAttributeDataType::HALF_FLOAT    , 1u, 1u * 2u, false}, // R16F
    {VertexAttributeDataType::FLOAT         , 1u, 1u * 4u, false}, // R32F
    {VertexAttributeDataType::HALF_FLOAT    , 2u, 2u * 2u, false}, // RG16F
    {VertexAttributeDataType::FLOAT         , 2u, 2u * 4u, false}, // RG32F
    {VertexAttributeDataType::HALF_FLOAT    , 3u, 3u * 2u, false}, // RGB16F
    {VertexAttributeDataType::FLOAT         , 3u, 3u * 4u, false}, // RGB32F
    {VertexAttributeDataType::HALF_FLOAT    , 4u, 4u * 2u, false}, // RGBA16F
    {VertexAttributeDataType::FLOAT         , 4u, 4u * 4u, false}  // RGBA32F
}};
// clang-format on

[[nodiscard]] inline VertexAttributeFormatDescriptor getVertexAttributeFormatDescriptor(VertexAttributeFormat format) {
    auto index = static_cast<size_t>(format);
    return vertexAttributeFormatDescriptors.at(index);
}

} // namespace nox
