#pragma once

#include <nox/common.h>

namespace NOX {

enum class FormatDataType : uint8_t {
    NONE,
    UINT,
    SINT,
    FLOAT
};

struct FormatDescriptor {
    FormatDataType dataType;
    uint8_t componentCount;
    uint8_t dataTypeSize;
    bool isNormalized;
    bool hasDepth;
    bool hasStencil;
};

namespace FormatHelper {

FormatDescriptor getFormatDescriptor(Format format);

} // namespace FormatHelper

} // namespace NOX
