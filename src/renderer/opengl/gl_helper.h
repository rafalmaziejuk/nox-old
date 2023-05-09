#pragma once

#include <nox/renderer/buffer_types.h>
#include <nox/renderer/shader_types.h>

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
GLenum mapDataTypeEnum(uint8_t type, uint8_t size);
GLenum mapShaderEnum(ShaderType type);
GLbitfield mapShaderBit(ShaderType type);

} // namespace NOX::GLHelper
