#pragma once

#include <glad/gl.h>

namespace NOX::GLHelper {

void GLAPIENTRY debugMessageCallback(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar *message,
                                     const void *userParam);

} // namespace NOX::GLHelper
