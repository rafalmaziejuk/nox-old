#include "renderer/format_helper.h"

namespace NOX::FormatHelper {

namespace {

uint8_t getComponentCountFromFormat(Format format) {
    auto value = static_cast<uint8_t>(format);
    value >>= FormatComponent::OFFSET;
    switch (value & FormatComponent::MASK) {
    case FormatComponent::R:
        return 1u;
    case FormatComponent::RG:
        return 2u;
    case FormatComponent::RGB:
        return 3u;
    case FormatComponent::RGBA:
        return 4u;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

uint8_t getDataTypeSizeFromFormat(Format format) {
    auto value = static_cast<uint8_t>(format);
    value >>= FormatBitsPerComponent::OFFSET;
    switch (value & FormatBitsPerComponent::MASK) {
    case FormatBitsPerComponent::BITS8:
        return 1u;
    case FormatBitsPerComponent::BITS16:
        return 2u;
    case FormatBitsPerComponent::BITS32:
        return 4u;
    case FormatBitsPerComponent::BITS64:
        return 8u;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

uint8_t getDataTypeFromFormat(Format format) {
    auto value = static_cast<uint8_t>(format);
    value >>= FormatDataType::OFFSET;
    switch (value & FormatDataType::MASK) {
    case FormatDataType::UINT:
        return FormatDataType::UINT;
    case FormatDataType::SINT:
        return FormatDataType::SINT;
    case FormatDataType::FLOAT:
        return FormatDataType::FLOAT;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

bool isDataTypeAttributePresentInFormat(Format format, uint8_t attribute) {
    auto value = static_cast<uint8_t>(format);
    value >>= FormatDataTypeAttribute::OFFSET;
    switch ((value & FormatDataTypeAttribute::MASK) & attribute) {
    case FormatDataTypeAttribute::NORMALIZED:
        return true;
    default:
        return false;
    }
}

} // namespace

FormatDescriptor getFormatDescriptor(Format format) {
    FormatDescriptor descriptor;
    descriptor.componentCount = getComponentCountFromFormat(format);
    descriptor.dataTypeSize = getDataTypeSizeFromFormat(format);
    descriptor.dataType = getDataTypeFromFormat(format);
    descriptor.isNormalized = isDataTypeAttributePresentInFormat(format, FormatDataTypeAttribute::NORMALIZED);
    return descriptor;
}

} // namespace NOX::FormatHelper
