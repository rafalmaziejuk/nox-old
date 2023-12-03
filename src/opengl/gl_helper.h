#pragma once

#include "format_helper.h"

#include <glad/gl.h>

namespace NOX::GLHelper {

GLenum mapFormatDataTypeToEnum(FormatDataType type, uint8_t size);

} // namespace NOX::GLHelper
