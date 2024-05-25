#include "src/format_descriptor.h"

#include <gtest/gtest.h>

#include <array>

using namespace nox;

TEST(FormatDescriptorTests, WhenGetImageFormatDescriptorIsCalledThenCorrectImageFormatDescriptorIsReturned) {
    using ImageFormatDescriptors = std::array<ImageFormatDescriptor, static_cast<size_t>(ImageFormat::MAX)>;
    constexpr ImageFormatDescriptors expectedImageFormatDescriptors{{
        {false, false, false, false}, // NONE
        {true, false, false, false},  // R8UI
        {true, false, false, false},  // R16UI
        {true, false, false, false},  // R32UI
        {true, false, false, false},  // RG8UI
        {true, false, false, false},  // RG16UI
        {true, false, false, false},  // RG32UI
        {true, false, false, false},  // RGB8UI
        {true, false, false, false},  // RGB16UI
        {true, false, false, false},  // RGB32UI
        {true, false, false, false},  // RGBA8UI
        {true, false, false, false},  // RGBA16UI
        {true, false, false, false},  // RGBA32UI
        {true, false, false, false},  // R8I
        {true, false, false, false},  // R16I
        {true, false, false, false},  // R32I
        {true, false, false, false},  // RG8I
        {true, false, false, false},  // RG16I
        {true, false, false, false},  // RG32I
        {true, false, false, false},  // RGB8I
        {true, false, false, false},  // RGB16I
        {true, false, false, false},  // RGB32I
        {true, false, false, false},  // RGBA8I
        {true, false, false, false},  // RGBA16I
        {true, false, false, false},  // RGBA32I
        {true, false, false, false},  // R8_UNORM
        {true, false, false, false},  // R16_UNORM
        {true, false, false, false},  // R32_UNORM
        {true, false, false, false},  // RG8_UNORM
        {true, false, false, false},  // RG16_UNORM
        {true, false, false, false},  // RG32_UNORM
        {true, false, false, false},  // RGB8_UNORM
        {true, false, false, false},  // RGB16_UNORM
        {true, false, false, false},  // RGB32_UNORM
        {true, false, false, false},  // RGBA8_UNORM
        {true, false, false, false},  // RGBA16_UNORM
        {true, false, false, false},  // RGBA32_UNORM
        {true, false, false, false},  // R8_SNORM
        {true, false, false, false},  // R16_SNORM
        {true, false, false, false},  // R32_SNORM
        {true, false, false, false},  // RG8_SNORM
        {true, false, false, false},  // RG16_SNORM
        {true, false, false, false},  // RG32_SNORM
        {true, false, false, false},  // RGB8_SNORM
        {true, false, false, false},  // RGB16_SNORM
        {true, false, false, false},  // RGB32_SNORM
        {true, false, false, false},  // RGBA8_SNORM
        {true, false, false, false},  // RGBA16_SNORM
        {true, false, false, false},  // RGBA32_SNORM
        {true, false, false, false},  // R16F
        {true, false, false, false},  // R32F
        {true, false, false, false},  // RG16F
        {true, false, false, false},  // RG32F
        {true, false, false, false},  // RGB16F
        {true, false, false, false},  // RGB32F
        {true, false, false, false},  // RGBA16F
        {true, false, false, false},  // RGBA32F
        {false, true, false, false},  // DEPTH16
        {false, true, false, false},  // DEPTH24
        {false, true, false, false},  // DEPTH32
        {false, true, false, false},  // DEPTH32F
        {false, false, true, false},  // STENCIL8
        {false, false, false, true},  // DEPTH24_STENCIL8
        {false, false, false, true}   // DEPTH32F_STENCIL8
    }};

    for (size_t i = static_cast<size_t>(ImageFormat::NONE); i < static_cast<size_t>(ImageFormat::MAX); i++) {
        const auto imageFormatDescriptor = getImageFormatDescriptor(static_cast<ImageFormat>(i));
        const auto &expectedImageFormatDescriptor = expectedImageFormatDescriptors.at(i);
        EXPECT_EQ(expectedImageFormatDescriptor.isColor, imageFormatDescriptor.isColor);
        EXPECT_EQ(expectedImageFormatDescriptor.isDepth, imageFormatDescriptor.isDepth);
        EXPECT_EQ(expectedImageFormatDescriptor.isStencil, imageFormatDescriptor.isStencil);
        EXPECT_EQ(expectedImageFormatDescriptor.isDepthStencil, imageFormatDescriptor.isDepthStencil);
    }
}

TEST(FormatDescriptorTests, WhenGetVertexAttributeFormatDescriptorIsCalledThenCorrectVertexAttributeFormatDescriptorIsReturned) {
    using VertexAttributeFormatDescriptors = std::array<VertexAttributeFormatDescriptor, static_cast<size_t>(VertexAttributeFormat::MAX)>;
    constexpr VertexAttributeFormatDescriptors expectedVertexAttributeFormatDescriptors{{
        {VertexAttributeType::NONE, 0u, 0u, false},                // NONE
        {VertexAttributeType::UNSIGNED_BYTE, 1u, 1u * 1u, false},  // R8UI
        {VertexAttributeType::UNSIGNED_SHORT, 1u, 1u * 2u, false}, // R16UI
        {VertexAttributeType::UNSIGNED_INT, 1u, 1u * 4u, false},   // R32UI
        {VertexAttributeType::UNSIGNED_BYTE, 2u, 2u * 1u, false},  // RG8UI
        {VertexAttributeType::UNSIGNED_SHORT, 2u, 2u * 2u, false}, // RG16UI
        {VertexAttributeType::UNSIGNED_INT, 2u, 2u * 4u, false},   // RG32UI
        {VertexAttributeType::UNSIGNED_BYTE, 3u, 3u * 1u, false},  // RGB8UI
        {VertexAttributeType::UNSIGNED_SHORT, 3u, 3u * 2u, false}, // RGB16UI
        {VertexAttributeType::UNSIGNED_INT, 3u, 3u * 4u, false},   // RGB32UI
        {VertexAttributeType::UNSIGNED_BYTE, 4u, 4u * 1u, false},  // RGBA8UI
        {VertexAttributeType::UNSIGNED_SHORT, 4u, 4u * 2u, false}, // RGBA16UI
        {VertexAttributeType::UNSIGNED_INT, 4u, 4u * 4u, false},   // RGBA32UI
        {VertexAttributeType::BYTE, 1u, 1u * 1u, false},           // R8I
        {VertexAttributeType::SHORT, 1u, 1u * 2u, false},          // R16I
        {VertexAttributeType::INT, 1u, 1u * 4u, false},            // R32I
        {VertexAttributeType::BYTE, 2u, 2u * 1u, false},           // RG8I
        {VertexAttributeType::SHORT, 2u, 2u * 2u, false},          // RG16I
        {VertexAttributeType::INT, 2u, 2u * 4u, false},            // RG32I
        {VertexAttributeType::BYTE, 3u, 3u * 1u, false},           // RGB8I
        {VertexAttributeType::SHORT, 3u, 3u * 2u, false},          // RGB16I
        {VertexAttributeType::INT, 3u, 3u * 4u, false},            // RGB32I
        {VertexAttributeType::BYTE, 4u, 4u * 1u, false},           // RGBA8I
        {VertexAttributeType::SHORT, 4u, 4u * 2u, false},          // RGBA16I
        {VertexAttributeType::INT, 4u, 4u * 4u, false},            // RGBA32I
        {VertexAttributeType::UNSIGNED_BYTE, 1u, 1u * 1u, true},   // R8_UNORM
        {VertexAttributeType::UNSIGNED_SHORT, 1u, 1u * 2u, true},  // R16_UNORM
        {VertexAttributeType::UNSIGNED_INT, 1u, 1u * 4u, true},    // R32_UNORM
        {VertexAttributeType::UNSIGNED_BYTE, 2u, 2u * 1u, true},   // RG8_UNORM
        {VertexAttributeType::UNSIGNED_SHORT, 2u, 2u * 2u, true},  // RG16_UNORM
        {VertexAttributeType::UNSIGNED_INT, 2u, 2u * 4u, true},    // RG32_UNORM
        {VertexAttributeType::UNSIGNED_BYTE, 3u, 3u * 1u, true},   // RGB8_UNORM
        {VertexAttributeType::UNSIGNED_SHORT, 3u, 3u * 2u, true},  // RGB16_UNORM
        {VertexAttributeType::UNSIGNED_INT, 3u, 3u * 4u, true},    // RGB32_UNORM
        {VertexAttributeType::UNSIGNED_BYTE, 4u, 4u * 1u, true},   // RGBA8_UNORM
        {VertexAttributeType::UNSIGNED_SHORT, 4u, 4u * 2u, true},  // RGBA16_UNORM
        {VertexAttributeType::UNSIGNED_INT, 4u, 4u * 4u, true},    // RGBA32_UNORM
        {VertexAttributeType::BYTE, 1u, 1u * 1u, true},            // R8_SNORM
        {VertexAttributeType::SHORT, 1u, 1u * 2u, true},           // R16_SNORM
        {VertexAttributeType::INT, 1u, 1u * 4u, true},             // R32_SNORM
        {VertexAttributeType::BYTE, 2u, 2u * 1u, true},            // RG8_SNORM
        {VertexAttributeType::SHORT, 2u, 2u * 2u, true},           // RG16_SNORM
        {VertexAttributeType::INT, 2u, 2u * 4u, true},             // RG32_SNORM
        {VertexAttributeType::BYTE, 3u, 3u * 1u, true},            // RGB8_SNORM
        {VertexAttributeType::SHORT, 3u, 3u * 2u, true},           // RGB16_SNORM
        {VertexAttributeType::INT, 3u, 3u * 4u, true},             // RGB32_SNORM
        {VertexAttributeType::BYTE, 4u, 4u * 1u, true},            // RGBA8_SNORM
        {VertexAttributeType::SHORT, 4u, 4u * 2u, true},           // RGBA16_SNORM
        {VertexAttributeType::INT, 4u, 4u * 4u, true},             // RGBA32_SNORM
        {VertexAttributeType::HALF_FLOAT, 1u, 1u * 2u, false},     // R16F
        {VertexAttributeType::FLOAT, 1u, 1u * 4u, false},          // R32F
        {VertexAttributeType::HALF_FLOAT, 2u, 2u * 2u, false},     // RG16F
        {VertexAttributeType::FLOAT, 2u, 2u * 4u, false},          // RG32F
        {VertexAttributeType::HALF_FLOAT, 3u, 3u * 2u, false},     // RGB16F
        {VertexAttributeType::FLOAT, 3u, 3u * 4u, false},          // RGB32F
        {VertexAttributeType::HALF_FLOAT, 4u, 4u * 2u, false},     // RGBA16F
        {VertexAttributeType::FLOAT, 4u, 4u * 4u, false}           // RGBA32F
    }};

    for (size_t i = static_cast<size_t>(VertexAttributeFormat::NONE); i < static_cast<size_t>(VertexAttributeFormat::MAX); i++) {
        const auto vertexAttributeFormatDescriptor = getVertexAttributeFormatDescriptor(static_cast<VertexAttributeFormat>(i));
        const auto &expectedVertexAttributeFormatDescriptor = expectedVertexAttributeFormatDescriptors.at(i);
        EXPECT_EQ(expectedVertexAttributeFormatDescriptor.vertexAttributeType, vertexAttributeFormatDescriptor.vertexAttributeType);
        EXPECT_EQ(expectedVertexAttributeFormatDescriptor.componentsCount, vertexAttributeFormatDescriptor.componentsCount);
        EXPECT_EQ(expectedVertexAttributeFormatDescriptor.stride, vertexAttributeFormatDescriptor.stride);
        EXPECT_EQ(expectedVertexAttributeFormatDescriptor.isNormalized, vertexAttributeFormatDescriptor.isNormalized);
    }
}
