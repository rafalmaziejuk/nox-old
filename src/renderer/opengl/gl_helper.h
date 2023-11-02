#pragma once

#include "renderer/format_helper.h"

#include <glad/gl.h>

#include <string>

namespace NOX::GLHelper {

GLenum mapFormatDataTypeToEnum(FormatDataType type, uint8_t size);

bool isExtensionSupported(std::string_view extensionsList, std::string_view extension);

} // namespace NOX::GLHelper
