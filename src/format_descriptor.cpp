#include "format_descriptor.h"

#include <array>
#include <cstddef>

namespace nox {

namespace {

using ImageFormatTypes = std::array<ImageFormatType, static_cast<size_t>(ImageFormat::MAX)>;

constexpr ImageFormatTypes imageFormatTypes{{
    ImageFormatType::NONE,          // NONE
    ImageFormatType::COLOR,         // R8UI
    ImageFormatType::COLOR,         // R16UI
    ImageFormatType::COLOR,         // R32UI
    ImageFormatType::COLOR,         // RG8UI
    ImageFormatType::COLOR,         // RG16UI
    ImageFormatType::COLOR,         // RG32UI
    ImageFormatType::COLOR,         // RGB8UI
    ImageFormatType::COLOR,         // RGB16UI
    ImageFormatType::COLOR,         // RGB32UI
    ImageFormatType::COLOR,         // RGBA8UI
    ImageFormatType::COLOR,         // RGBA16UI
    ImageFormatType::COLOR,         // RGBA32UI
    ImageFormatType::COLOR,         // R8I
    ImageFormatType::COLOR,         // R16I
    ImageFormatType::COLOR,         // R32I
    ImageFormatType::COLOR,         // RG8I
    ImageFormatType::COLOR,         // RG16I
    ImageFormatType::COLOR,         // RG32I
    ImageFormatType::COLOR,         // RGB8I
    ImageFormatType::COLOR,         // RGB16I
    ImageFormatType::COLOR,         // RGB32I
    ImageFormatType::COLOR,         // RGBA8I
    ImageFormatType::COLOR,         // RGBA16I
    ImageFormatType::COLOR,         // RGBA32I
    ImageFormatType::COLOR,         // R8_UNORM
    ImageFormatType::COLOR,         // R16_UNORM
    ImageFormatType::COLOR,         // R32_UNORM
    ImageFormatType::COLOR,         // RG8_UNORM
    ImageFormatType::COLOR,         // RG16_UNORM
    ImageFormatType::COLOR,         // RG32_UNORM
    ImageFormatType::COLOR,         // RGB8_UNORM
    ImageFormatType::COLOR,         // RGB16_UNORM
    ImageFormatType::COLOR,         // RGB32_UNORM
    ImageFormatType::COLOR,         // RGBA8_UNORM
    ImageFormatType::COLOR,         // RGBA16_UNORM
    ImageFormatType::COLOR,         // RGBA32_UNORM
    ImageFormatType::COLOR,         // R8_SNORM
    ImageFormatType::COLOR,         // R16_SNORM
    ImageFormatType::COLOR,         // R32_SNORM
    ImageFormatType::COLOR,         // RG8_SNORM
    ImageFormatType::COLOR,         // RG16_SNORM
    ImageFormatType::COLOR,         // RG32_SNORM
    ImageFormatType::COLOR,         // RGB8_SNORM
    ImageFormatType::COLOR,         // RGB16_SNORM
    ImageFormatType::COLOR,         // RGB32_SNORM
    ImageFormatType::COLOR,         // RGBA8_SNORM
    ImageFormatType::COLOR,         // RGBA16_SNORM
    ImageFormatType::COLOR,         // RGBA32_SNORM
    ImageFormatType::COLOR,         // R16F
    ImageFormatType::COLOR,         // R32F
    ImageFormatType::COLOR,         // RG16F
    ImageFormatType::COLOR,         // RG32F
    ImageFormatType::COLOR,         // RGB16F
    ImageFormatType::COLOR,         // RGB32F
    ImageFormatType::COLOR,         // RGBA16F
    ImageFormatType::COLOR,         // RGBA32F
    ImageFormatType::DEPTH,         // DEPTH16
    ImageFormatType::DEPTH,         // DEPTH24
    ImageFormatType::DEPTH,         // DEPTH32
    ImageFormatType::DEPTH,         // DEPTH32F
    ImageFormatType::STENCIL,       // STENCIL8
    ImageFormatType::DEPTH_STENCIL, // DEPTH24_STENCIL8
    ImageFormatType::DEPTH_STENCIL  // DEPTH32F_STENCIL8
}};

using VertexAttributeFormatDescriptors = std::array<VertexAttributeFormatDescriptor, static_cast<size_t>(VertexAttributeFormat::MAX)>;

// clang-format off
constexpr VertexAttributeFormatDescriptors vertexAttributeFormatDescriptors{{
    {VertexAttributeType::NONE          , 0u, 0u     , false}, // NONE
    {VertexAttributeType::UNSIGNED_BYTE , 1u, 1u * 1u, false}, // R8UI
    {VertexAttributeType::UNSIGNED_SHORT, 1u, 1u * 2u, false}, // R16UI
    {VertexAttributeType::UNSIGNED_INT  , 1u, 1u * 4u, false}, // R32UI
    {VertexAttributeType::UNSIGNED_BYTE , 2u, 2u * 1u, false}, // RG8UI
    {VertexAttributeType::UNSIGNED_SHORT, 2u, 2u * 2u, false}, // RG16UI
    {VertexAttributeType::UNSIGNED_INT  , 2u, 2u * 4u, false}, // RG32UI
    {VertexAttributeType::UNSIGNED_BYTE , 3u, 3u * 1u, false}, // RGB8UI
    {VertexAttributeType::UNSIGNED_SHORT, 3u, 3u * 2u, false}, // RGB16UI
    {VertexAttributeType::UNSIGNED_INT  , 3u, 3u * 4u, false}, // RGB32UI
    {VertexAttributeType::UNSIGNED_BYTE , 4u, 4u * 1u, false}, // RGBA8UI
    {VertexAttributeType::UNSIGNED_SHORT, 4u, 4u * 2u, false}, // RGBA16UI
    {VertexAttributeType::UNSIGNED_INT  , 4u, 4u * 4u, false}, // RGBA32UI
    {VertexAttributeType::BYTE          , 1u, 1u * 1u, false}, // R8I
    {VertexAttributeType::SHORT         , 1u, 1u * 2u, false}, // R16I
    {VertexAttributeType::INT           , 1u, 1u * 4u, false}, // R32I
    {VertexAttributeType::BYTE          , 2u, 2u * 1u, false}, // RG8I
    {VertexAttributeType::SHORT         , 2u, 2u * 2u, false}, // RG16I
    {VertexAttributeType::INT           , 2u, 2u * 4u, false}, // RG32I
    {VertexAttributeType::BYTE          , 3u, 3u * 1u, false}, // RGB8I
    {VertexAttributeType::SHORT         , 3u, 3u * 2u, false}, // RGB16I
    {VertexAttributeType::INT           , 3u, 3u * 4u, false}, // RGB32I
    {VertexAttributeType::BYTE          , 4u, 4u * 1u, false}, // RGBA8I
    {VertexAttributeType::SHORT         , 4u, 4u * 2u, false}, // RGBA16I
    {VertexAttributeType::INT           , 4u, 4u * 4u, false}, // RGBA32I
    {VertexAttributeType::UNSIGNED_BYTE , 1u, 1u * 1u, true }, // R8_UNORM
    {VertexAttributeType::UNSIGNED_SHORT, 1u, 1u * 2u, true }, // R16_UNORM
    {VertexAttributeType::UNSIGNED_INT  , 1u, 1u * 4u, true }, // R32_UNORM
    {VertexAttributeType::UNSIGNED_BYTE , 2u, 2u * 1u, true }, // RG8_UNORM
    {VertexAttributeType::UNSIGNED_SHORT, 2u, 2u * 2u, true }, // RG16_UNORM
    {VertexAttributeType::UNSIGNED_INT  , 2u, 2u * 4u, true }, // RG32_UNORM
    {VertexAttributeType::UNSIGNED_BYTE , 3u, 3u * 1u, true }, // RGB8_UNORM
    {VertexAttributeType::UNSIGNED_SHORT, 3u, 3u * 2u, true }, // RGB16_UNORM
    {VertexAttributeType::UNSIGNED_INT  , 3u, 3u * 4u, true }, // RGB32_UNORM
    {VertexAttributeType::UNSIGNED_BYTE , 4u, 4u * 1u, true }, // RGBA8_UNORM
    {VertexAttributeType::UNSIGNED_SHORT, 4u, 4u * 2u, true }, // RGBA16_UNORM
    {VertexAttributeType::UNSIGNED_INT  , 4u, 4u * 4u, true }, // RGBA32_UNORM
    {VertexAttributeType::BYTE          , 1u, 1u * 1u, true }, // R8_SNORM
    {VertexAttributeType::SHORT         , 1u, 1u * 2u, true }, // R16_SNORM
    {VertexAttributeType::INT           , 1u, 1u * 4u, true }, // R32_SNORM
    {VertexAttributeType::BYTE          , 2u, 2u * 1u, true }, // RG8_SNORM
    {VertexAttributeType::SHORT         , 2u, 2u * 2u, true }, // RG16_SNORM
    {VertexAttributeType::INT           , 2u, 2u * 4u, true }, // RG32_SNORM
    {VertexAttributeType::BYTE          , 3u, 3u * 1u, true }, // RGB8_SNORM
    {VertexAttributeType::SHORT         , 3u, 3u * 2u, true }, // RGB16_SNORM
    {VertexAttributeType::INT           , 3u, 3u * 4u, true }, // RGB32_SNORM
    {VertexAttributeType::BYTE          , 4u, 4u * 1u, true }, // RGBA8_SNORM
    {VertexAttributeType::SHORT         , 4u, 4u * 2u, true }, // RGBA16_SNORM
    {VertexAttributeType::INT           , 4u, 4u * 4u, true }, // RGBA32_SNORM
    {VertexAttributeType::HALF_FLOAT    , 1u, 1u * 2u, false}, // R16F
    {VertexAttributeType::FLOAT         , 1u, 1u * 4u, false}, // R32F
    {VertexAttributeType::HALF_FLOAT    , 2u, 2u * 2u, false}, // RG16F
    {VertexAttributeType::FLOAT         , 2u, 2u * 4u, false}, // RG32F
    {VertexAttributeType::HALF_FLOAT    , 3u, 3u * 2u, false}, // RGB16F
    {VertexAttributeType::FLOAT         , 3u, 3u * 4u, false}, // RGB32F
    {VertexAttributeType::HALF_FLOAT    , 4u, 4u * 2u, false}, // RGBA16F
    {VertexAttributeType::FLOAT         , 4u, 4u * 4u, false}  // RGBA32F
}};
// clang-format on

} // namespace

ImageFormatType getImageFormatType(ImageFormat format) {
    auto index = static_cast<size_t>(format);
    return imageFormatTypes.at(index);
}

VertexAttributeFormatDescriptor getVertexAttributeFormatDescriptor(VertexAttributeFormat format) {
    auto index = static_cast<size_t>(format);
    return vertexAttributeFormatDescriptors.at(index);
}

} // namespace nox
