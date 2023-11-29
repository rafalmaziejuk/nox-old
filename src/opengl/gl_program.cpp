#include "opengl/gl_program.h"
#include "opengl/gl_shader.h"

#include <glad/gl.h>

#include <cstddef>
#include <vector>

namespace NOX {

GLProgram::GLProgram() {
    m_handle = glCreateProgram();
    glProgramParameteri(m_handle, GL_PROGRAM_SEPARABLE, GL_TRUE);
}

GLProgram::~GLProgram() {
    glDeleteProgram(m_handle);
}

void GLProgram::attachShader(uint32_t shaderHandle) const {
    glAttachShader(m_handle, shaderHandle);
}

bool GLProgram::link() const {
    glLinkProgram(m_handle);

    GLint attachedShadersCount = 0;
    glGetProgramiv(m_handle, GL_ATTACHED_SHADERS, &attachedShadersCount);

    std::vector<GLuint> handles(static_cast<size_t>(attachedShadersCount));
    glGetAttachedShaders(m_handle, attachedShadersCount, nullptr, handles.data());

    for (const auto handle : handles) {
        glDetachShader(m_handle, handle);
    }

    GLint result = GL_TRUE;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &result);

    if (result == GL_FALSE) {
        glDeleteProgram(m_handle);
        return false;
    }

    return true;
}

} // namespace NOX
