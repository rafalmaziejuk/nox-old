#include "renderer/format_helper.h"

namespace NOX::FormatHelper {

namespace {

struct FormatAttribute {
    enum {
        R = BIT(0),
        RG = BIT(1),
        RGB = BIT(2),
        RGBA = BIT(3),

        BITS8 = BIT(4),
        BITS16 = BIT(5),
        BITS24 = BIT(6),
        BITS32 = BIT(7),
        BITS64 = BIT(8),

        UINT = BIT(9),
        SINT = BIT(10),
        FLOAT = BIT(11),

        NORMALIZED = BIT(12),
        DEPTH = BIT(13),
        STENCIL = BIT(14)
    };
};

constexpr std::array<uint32_t, static_cast<size_t>(Format::MAX)> formatValues{{
    0u,                                                                   // NONE
    FormatAttribute::R | FormatAttribute::BITS8 | FormatAttribute::UINT,  // R8_UINT
    FormatAttribute::R | FormatAttribute::BITS16 | FormatAttribute::UINT, // R16_UINT
    FormatAttribute::R | FormatAttribute::BITS32 | FormatAttribute::UINT, // R32_UINT

    FormatAttribute::RG | FormatAttribute::BITS8 | FormatAttribute::UINT,  // RG8_UINT
    FormatAttribute::RG | FormatAttribute::BITS16 | FormatAttribute::UINT, // RG16_UINT
    FormatAttribute::RG | FormatAttribute::BITS32 | FormatAttribute::UINT, // RG32_UINT

    FormatAttribute::RGB | FormatAttribute::BITS8 | FormatAttribute::UINT,  // RGB8_UINT
    FormatAttribute::RGB | FormatAttribute::BITS16 | FormatAttribute::UINT, // RGB16_UINT
    FormatAttribute::RGB | FormatAttribute::BITS32 | FormatAttribute::UINT, // RGB32_UINT

    FormatAttribute::RGBA | FormatAttribute::BITS8 | FormatAttribute::UINT,  // RGBA8_UINT
    FormatAttribute::RGBA | FormatAttribute::BITS16 | FormatAttribute::UINT, // RGBA16_UINT
    FormatAttribute::RGBA | FormatAttribute::BITS32 | FormatAttribute::UINT, // RGBA32_UINT

    FormatAttribute::R | FormatAttribute::BITS8 | FormatAttribute::SINT,  // R8_SINT
    FormatAttribute::R | FormatAttribute::BITS16 | FormatAttribute::SINT, // R16_SINT
    FormatAttribute::R | FormatAttribute::BITS32 | FormatAttribute::SINT, // R32_SINT

    FormatAttribute::RG | FormatAttribute::BITS8 | FormatAttribute::SINT,  // RG8_SINT
    FormatAttribute::RG | FormatAttribute::BITS16 | FormatAttribute::SINT, // RG16_SINT
    FormatAttribute::RG | FormatAttribute::BITS32 | FormatAttribute::SINT, // RG32_SINT

    FormatAttribute::RGB | FormatAttribute::BITS8 | FormatAttribute::SINT,  // RGB8_SINT
    FormatAttribute::RGB | FormatAttribute::BITS16 | FormatAttribute::SINT, // RGB16_SINT
    FormatAttribute::RGB | FormatAttribute::BITS32 | FormatAttribute::SINT, // RGB32_SINT

    FormatAttribute::RGBA | FormatAttribute::BITS8 | FormatAttribute::SINT,  // RGBA8_SINT
    FormatAttribute::RGBA | FormatAttribute::BITS16 | FormatAttribute::SINT, // RGBA16_SINT
    FormatAttribute::RGBA | FormatAttribute::BITS32 | FormatAttribute::SINT, // RGBA32_SINT

    FormatAttribute::R | FormatAttribute::BITS8 | FormatAttribute::UINT | FormatAttribute::NORMALIZED,  // R8_UINT_NORM
    FormatAttribute::R | FormatAttribute::BITS16 | FormatAttribute::UINT | FormatAttribute::NORMALIZED, // R16_UINT_NORM
    FormatAttribute::R | FormatAttribute::BITS32 | FormatAttribute::UINT | FormatAttribute::NORMALIZED, // R32_UINT_NORM

    FormatAttribute::RG | FormatAttribute::BITS8 | FormatAttribute::UINT | FormatAttribute::NORMALIZED,  // RG8_UINT_NORM
    FormatAttribute::RG | FormatAttribute::BITS16 | FormatAttribute::UINT | FormatAttribute::NORMALIZED, // RG16_UINT_NORM
    FormatAttribute::RG | FormatAttribute::BITS32 | FormatAttribute::UINT | FormatAttribute::NORMALIZED, // RG32_UINT_NORM

    FormatAttribute::RGB | FormatAttribute::BITS8 | FormatAttribute::UINT | FormatAttribute::NORMALIZED,  // RGB8_UINT_NORM
    FormatAttribute::RGB | FormatAttribute::BITS16 | FormatAttribute::UINT | FormatAttribute::NORMALIZED, // RGB16_UINT_NORM
    FormatAttribute::RGB | FormatAttribute::BITS32 | FormatAttribute::UINT | FormatAttribute::NORMALIZED, // RGB32_UINT_NORM

    FormatAttribute::RGBA | FormatAttribute::BITS8 | FormatAttribute::UINT | FormatAttribute::NORMALIZED,  // RGBA8_UINT_NORM
    FormatAttribute::RGBA | FormatAttribute::BITS16 | FormatAttribute::UINT | FormatAttribute::NORMALIZED, // RGBA16_UINT_NORM
    FormatAttribute::RGBA | FormatAttribute::BITS32 | FormatAttribute::UINT | FormatAttribute::NORMALIZED, // RGBA32_UINT_NORM

    FormatAttribute::R | FormatAttribute::BITS8 | FormatAttribute::SINT | FormatAttribute::NORMALIZED,  // R8_SNORM
    FormatAttribute::R | FormatAttribute::BITS16 | FormatAttribute::SINT | FormatAttribute::NORMALIZED, // R16_SNORM
    FormatAttribute::R | FormatAttribute::BITS32 | FormatAttribute::SINT | FormatAttribute::NORMALIZED, // R32_SNORM

    FormatAttribute::RG | FormatAttribute::BITS8 | FormatAttribute::SINT | FormatAttribute::NORMALIZED,  // RG8_SNORM
    FormatAttribute::RG | FormatAttribute::BITS16 | FormatAttribute::SINT | FormatAttribute::NORMALIZED, // RG16_SNORM
    FormatAttribute::RG | FormatAttribute::BITS32 | FormatAttribute::SINT | FormatAttribute::NORMALIZED, // RG32_SNORM

    FormatAttribute::RGB | FormatAttribute::BITS8 | FormatAttribute::SINT | FormatAttribute::NORMALIZED,  // RGB8_SNORM
    FormatAttribute::RGB | FormatAttribute::BITS16 | FormatAttribute::SINT | FormatAttribute::NORMALIZED, // RGB16_SNORM
    FormatAttribute::RGB | FormatAttribute::BITS32 | FormatAttribute::SINT | FormatAttribute::NORMALIZED, // RGB32_SNORM

    FormatAttribute::RGBA | FormatAttribute::BITS8 | FormatAttribute::SINT | FormatAttribute::NORMALIZED,  // RGBA8_SNORM
    FormatAttribute::RGBA | FormatAttribute::BITS16 | FormatAttribute::SINT | FormatAttribute::NORMALIZED, // RGBA16_SNORM
    FormatAttribute::RGBA | FormatAttribute::BITS32 | FormatAttribute::SINT | FormatAttribute::NORMALIZED, // RGBA32_SNORM

    FormatAttribute::R | FormatAttribute::BITS16 | FormatAttribute::FLOAT, // R16_FLOAT
    FormatAttribute::R | FormatAttribute::BITS32 | FormatAttribute::FLOAT, // R32_FLOAT
    FormatAttribute::R | FormatAttribute::BITS64 | FormatAttribute::FLOAT, // R64_FLOAT

    FormatAttribute::RG | FormatAttribute::BITS16 | FormatAttribute::FLOAT, // RG16_FLOAT
    FormatAttribute::RG | FormatAttribute::BITS32 | FormatAttribute::FLOAT, // RG32_FLOAT
    FormatAttribute::RG | FormatAttribute::BITS64 | FormatAttribute::FLOAT, // RG64_FLOAT

    FormatAttribute::RGB | FormatAttribute::BITS16 | FormatAttribute::FLOAT, // RGB16_FLOAT
    FormatAttribute::RGB | FormatAttribute::BITS32 | FormatAttribute::FLOAT, // RGB32_FLOAT
    FormatAttribute::RGB | FormatAttribute::BITS64 | FormatAttribute::FLOAT, // RGB64_FLOAT

    FormatAttribute::RGBA | FormatAttribute::BITS16 | FormatAttribute::FLOAT, // RGBA16_FLOAT
    FormatAttribute::RGBA | FormatAttribute::BITS32 | FormatAttribute::FLOAT, // RGBA32_FLOAT
    FormatAttribute::RGBA | FormatAttribute::BITS64 | FormatAttribute::FLOAT, // RGBA64_FLOAT

    FormatAttribute::BITS16 | FormatAttribute::SINT | FormatAttribute::NORMALIZED | FormatAttribute::DEPTH, // DEPTH16I
    FormatAttribute::BITS24 | FormatAttribute::SINT | FormatAttribute::NORMALIZED | FormatAttribute::DEPTH, // DEPTH24I
    FormatAttribute::BITS32 | FormatAttribute::SINT | FormatAttribute::NORMALIZED | FormatAttribute::DEPTH, // DEPTH32I
    FormatAttribute::BITS32 | FormatAttribute::FLOAT | FormatAttribute::DEPTH,                              // DEPTH32F

    FormatAttribute::BITS8 | FormatAttribute::UINT | FormatAttribute::STENCIL, // STENCIL8UI

    FormatAttribute::BITS24 | FormatAttribute::SINT | FormatAttribute::NORMALIZED | FormatAttribute::DEPTH | FormatAttribute::STENCIL, // DEPTH24_STENCIL8
    FormatAttribute::BITS32 | FormatAttribute::FLOAT | FormatAttribute::DEPTH | FormatAttribute::STENCIL,                              // DEPTH32_FLOAT_STENCIL8
}};

FormatDataType getDataType(Format format) {
    auto value = formatValues.at(static_cast<size_t>(format));
    if (value & FormatAttribute::UINT) {
        return FormatDataType::UINT;
    }
    if (value & FormatAttribute::SINT) {
        return FormatDataType::SINT;
    }
    if (value & FormatAttribute::FLOAT) {
        return FormatDataType::FLOAT;
    }

    return FormatDataType::NONE;
}

uint8_t getComponentCount(Format format) {
    auto value = formatValues.at(static_cast<size_t>(format));
    if (value & FormatAttribute::R) {
        return 1u;
    }
    if (value & FormatAttribute::RG) {
        return 2u;
    }
    if (value & FormatAttribute::RGB) {
        return 3u;
    }
    if (value & FormatAttribute::RGBA) {
        return 4u;
    }

    return 0u;
}

uint8_t getDataTypeSize(Format format) {
    auto value = formatValues.at(static_cast<size_t>(format));
    if (value & FormatAttribute::BITS8) {
        return 1u;
    }
    if (value & FormatAttribute::BITS16) {
        return 2u;
    }
    if (value & FormatAttribute::BITS24) {
        return 3u;
    }
    if (value & FormatAttribute::BITS32) {
        return 4u;
    }
    if (value & FormatAttribute::BITS64) {
        return 8u;
    }

    return 0u;
}

bool isDataTypeAttributeEnabled(Format format, uint32_t attribute) {
    auto value = formatValues.at(static_cast<size_t>(format));
    switch (value & attribute) {
    case FormatAttribute::NORMALIZED:
    case FormatAttribute::DEPTH:
    case FormatAttribute::STENCIL:
        return true;
    }

    return false;
}

} // namespace

FormatDescriptor getFormatDescriptor(Format format) {
    FormatDescriptor descriptor{};
    descriptor.dataType = getDataType(format);
    descriptor.componentCount = getComponentCount(format);
    descriptor.dataTypeSize = getDataTypeSize(format);
    descriptor.isNormalized = isDataTypeAttributeEnabled(format, FormatAttribute::NORMALIZED);
    descriptor.hasDepth = isDataTypeAttributeEnabled(format, FormatAttribute::DEPTH);
    descriptor.hasStencil = isDataTypeAttributeEnabled(format, FormatAttribute::STENCIL);
    return descriptor;
}

} // namespace NOX::FormatHelper
