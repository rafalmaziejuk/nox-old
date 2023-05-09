#pragma once

#include <nox/export.h>
#include <nox/renderer/format.h>

namespace NOX::FormatHelper {

struct FormatDescriptor {
    Format format;
    uint8_t componentCount;
    uint8_t dataTypeSize;
    uint8_t dataType;
    bool isNormalized;
};

NOX_EXPORT FormatDescriptor getFormatDescriptor(Format format);

} // namespace NOX::FormatHelper
