#pragma once

#include "renderer/format_helper.h"

#include <nox/buffer_types.h>
#include <nox/pipeline_state_types.h>
#include <nox/shader_types.h>

#include <glad/gl.h>

namespace NOX::GLHelper {

void GLAPIENTRY debugMessageCallback(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar *message,
                                     const void *userParam);

GLbitfield mapBufferStorageFlags(uint8_t accessMethod);
GLenum mapDataTypeEnum(FormatDataType type, uint8_t size);
GLenum mapShaderEnum(ShaderType type);
GLbitfield mapShaderBit(ShaderType type);
GLenum mapPrimitiveTopologyEnum(PrimitiveTopology topology);
GLbitfield mapClearFlagBits(uint8_t clearFlag);

} // namespace NOX::GLHelper
