#pragma once

#include "renderer/format_helper.h"

#include <nox/buffer_types.h>
#include <nox/pipeline_state_types.h>
#include <nox/shader_types.h>

#include <glad/gl.h>

namespace NOX::GLHelper {

GLenum mapFormatDataTypeToEnum(FormatDataType type, uint8_t size);

} // namespace NOX::GLHelper
