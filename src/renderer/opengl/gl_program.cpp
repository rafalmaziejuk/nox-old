#include "core/core.h"
#include "renderer/opengl/gl_program.h"
#include "renderer/opengl/gl_shader.h"

#include <glad/gl.h>

namespace NOX {

GLProgram::GLProgram() {
    m_handle = glCreateProgram();
    glProgramParameteri(m_handle, GL_PROGRAM_SEPARABLE, GL_TRUE);
}

GLProgram::~GLProgram() {
    glDeleteProgram(m_handle);
}

uint32_t GLProgram::attachShader(const Shader *shader) {
    const auto *glShader = downcast<GLShader>(*shader);

    glAttachShader(m_handle, glShader->getHandle());
    m_attachedShaderHandles.push_back(glShader->getHandle());
    return glShader->getStageBit();
}

void GLProgram::link() {
    glLinkProgram(m_handle);

    for (const auto shaderHandle : m_attachedShaderHandles) {
        glDetachShader(m_handle, shaderHandle);
    }
    m_attachedShaderHandles.clear();

    GLint result = GL_TRUE;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &result);

    if (result == GL_FALSE) {
        glDeleteProgram(m_handle);
    }
}

} // namespace NOX
