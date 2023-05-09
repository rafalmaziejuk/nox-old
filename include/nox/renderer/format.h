#pragma once

#include <cstdint>

namespace NOX {

struct FormatComponent {
    enum {
        R,
        RG,
        RGB,
        RGBA,
        OFFSET = 0,
        MASK = 0b11
    };

    static constexpr auto valueR = FormatComponent::R << FormatComponent::OFFSET;
    static constexpr auto valueRG = FormatComponent::RG << FormatComponent::OFFSET;
    static constexpr auto valueRGB = FormatComponent::RGB << FormatComponent::OFFSET;
    static constexpr auto valueRGBA = FormatComponent::RGBA << FormatComponent::OFFSET;
};

struct FormatBitsPerComponent {
    enum {
        BITS8,
        BITS16,
        BITS32,
        BITS64,
        OFFSET = 2,
        MASK = 0b11
    };

    static constexpr auto valueBits8 = FormatBitsPerComponent::BITS8 << FormatBitsPerComponent::OFFSET;
    static constexpr auto valueBits16 = FormatBitsPerComponent::BITS16 << FormatBitsPerComponent::OFFSET;
    static constexpr auto valueBits32 = FormatBitsPerComponent::BITS32 << FormatBitsPerComponent::OFFSET;
    static constexpr auto valueBits64 = FormatBitsPerComponent::BITS64 << FormatBitsPerComponent::OFFSET;
};

struct FormatDataType {
    enum {
        UINT,
        SINT,
        FLOAT,
        OFFSET = 4,
        MASK = 0b11
    };

    static constexpr auto valueUInt = FormatDataType::UINT << FormatDataType::OFFSET;
    static constexpr auto valueSInt = FormatDataType::SINT << FormatDataType::OFFSET;
    static constexpr auto valueFloat = FormatDataType::FLOAT << FormatDataType::OFFSET;
};

struct FormatDataTypeAttribute {
    enum {
        NONE,
        NORMALIZED,
        OFFSET = 6,
        MASK = 0b1
    };

    static constexpr auto valueNormalized = FormatDataTypeAttribute::NORMALIZED << FormatDataTypeAttribute::OFFSET;
};

enum class Format {
    R8_UINT = FormatComponent::valueR | FormatBitsPerComponent::valueBits8 | FormatDataType::valueUInt,
    R16_UINT = FormatComponent::valueR | FormatBitsPerComponent::valueBits16 | FormatDataType::valueUInt,
    R32_UINT = FormatComponent::valueR | FormatBitsPerComponent::valueBits32 | FormatDataType::valueUInt,

    RG8_UINT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits8 | FormatDataType::valueUInt,
    RG16_UINT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits16 | FormatDataType::valueUInt,
    RG32_UINT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits32 | FormatDataType::valueUInt,

    RGB8_UINT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits8 | FormatDataType::valueUInt,
    RGB16_UINT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits16 | FormatDataType::valueUInt,
    RGB32_UINT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits32 | FormatDataType::valueUInt,

    RGBA8_UINT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits8 | FormatDataType::valueUInt,
    RGBA16_UINT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits16 | FormatDataType::valueUInt,
    RGBA32_UINT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits32 | FormatDataType::valueUInt,

    R8_SINT = FormatComponent::valueR | FormatBitsPerComponent::valueBits8 | FormatDataType::valueSInt,
    R16_SINT = FormatComponent::valueR | FormatBitsPerComponent::valueBits16 | FormatDataType::valueSInt,
    R32_SINT = FormatComponent::valueR | FormatBitsPerComponent::valueBits32 | FormatDataType::valueSInt,

    RG8_SINT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits8 | FormatDataType::valueSInt,
    RG16_SINT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits16 | FormatDataType::valueSInt,
    RG32_SINT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits32 | FormatDataType::valueSInt,

    RGB8_SINT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits8 | FormatDataType::valueSInt,
    RGB16_SINT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits16 | FormatDataType::valueSInt,
    RGB32_SINT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits32 | FormatDataType::valueSInt,

    RGBA8_SINT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits8 | FormatDataType::valueSInt,
    RGBA16_SINT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits16 | FormatDataType::valueSInt,
    RGBA32_SINT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits32 | FormatDataType::valueSInt,

    R8_UINT_NORM = R8_UINT | FormatDataTypeAttribute::valueNormalized,
    R16_UINT_NORM = R16_UINT | FormatDataTypeAttribute::valueNormalized,
    R32_UINT_NORM = R32_UINT | FormatDataTypeAttribute::valueNormalized,

    RG8_UINT_NORM = RG8_UINT | FormatDataTypeAttribute::valueNormalized,
    RG16_UINT_NORM = RG16_UINT | FormatDataTypeAttribute::valueNormalized,
    RG32_UINT_NORM = RG32_UINT | FormatDataTypeAttribute::valueNormalized,

    RGB8_UINT_NORM = RGB8_UINT | FormatDataTypeAttribute::valueNormalized,
    RGB16_UINT_NORM = RGB16_UINT | FormatDataTypeAttribute::valueNormalized,
    RGB32_UINT_NORM = RGB32_UINT | FormatDataTypeAttribute::valueNormalized,

    RGBA8_UINT_NORM = RGBA8_UINT | FormatDataTypeAttribute::valueNormalized,
    RGBA16_UINT_NORM = RGBA16_UINT | FormatDataTypeAttribute::valueNormalized,
    RGBA32_UINT_NORM = RGBA32_UINT | FormatDataTypeAttribute::valueNormalized,

    R8_SINT_NORM = R8_SINT | FormatDataTypeAttribute::valueNormalized,
    R16_SINT_NORM = R16_SINT | FormatDataTypeAttribute::valueNormalized,
    R32_SINT_NORM = R32_SINT | FormatDataTypeAttribute::valueNormalized,

    RG8_SINT_NORM = RG8_SINT | FormatDataTypeAttribute::valueNormalized,
    RG16_SINT_NORM = RG16_SINT | FormatDataTypeAttribute::valueNormalized,
    RG32_SINT_NORM = RG32_SINT | FormatDataTypeAttribute::valueNormalized,

    RGB8_SINT_NORM = RGB8_SINT | FormatDataTypeAttribute::valueNormalized,
    RGB16_SINT_NORM = RGB16_SINT | FormatDataTypeAttribute::valueNormalized,
    RGB32_SINT_NORM = RGB32_SINT | FormatDataTypeAttribute::valueNormalized,

    RGBA8_SINT_NORM = RGBA8_SINT | FormatDataTypeAttribute::valueNormalized,
    RGBA16_SINT_NORM = RGBA16_SINT | FormatDataTypeAttribute::valueNormalized,
    RGBA32_SINT_NORM = RGBA32_SINT | FormatDataTypeAttribute::valueNormalized,

    R16_FLOAT = FormatComponent::valueR | FormatBitsPerComponent::valueBits16 | FormatDataType::valueFloat,
    R32_FLOAT = FormatComponent::valueR | FormatBitsPerComponent::valueBits32 | FormatDataType::valueFloat,
    R64_FLOAT = FormatComponent::valueR | FormatBitsPerComponent::valueBits64 | FormatDataType::valueFloat,

    RG16_FLOAT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits16 | FormatDataType::valueFloat,
    RG32_FLOAT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits32 | FormatDataType::valueFloat,
    RG64_FLOAT = FormatComponent::valueRG | FormatBitsPerComponent::valueBits64 | FormatDataType::valueFloat,

    RGB16_FLOAT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits16 | FormatDataType::valueFloat,
    RGB32_FLOAT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits32 | FormatDataType::valueFloat,
    RGB64_FLOAT = FormatComponent::valueRGB | FormatBitsPerComponent::valueBits64 | FormatDataType::valueFloat,

    RGBA16_FLOAT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits16 | FormatDataType::valueFloat,
    RGBA32_FLOAT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits32 | FormatDataType::valueFloat,
    RGBA64_FLOAT = FormatComponent::valueRGBA | FormatBitsPerComponent::valueBits64 | FormatDataType::valueFloat
};

} // namespace NOX
