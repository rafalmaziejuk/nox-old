#pragma once

#include <nox/export.h>
#include <nox/format.h>

namespace NOX {

enum class FormatDataType : uint8_t {
    NONE,
    UINT,
    SINT,
    FLOAT
};

struct FormatDescriptor {
    Format format;
    FormatDataType dataType;
    uint8_t componentCount;
    uint8_t dataTypeSize;
    bool isNormalized;
    bool hasDepth;
    bool hasStencil;
};

namespace FormatHelper {

NOX_EXPORT FormatDescriptor getFormatDescriptor(Format format);

} // namespace FormatHelper

} // namespace NOX
