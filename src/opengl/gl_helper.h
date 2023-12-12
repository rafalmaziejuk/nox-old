#pragma once

#include "format_helper.h"

#include <glad/gl.h>

namespace nox::GLHelper {

GLenum mapFormatDataTypeToEnum(FormatDataType type, uint8_t size);

} // namespace nox::GLHelper
