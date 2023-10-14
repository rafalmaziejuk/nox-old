#include "renderer/opengl/gl_program.h"
#include "renderer/opengl/gl_shader.h"

#include <glad/gl.h>

namespace NOX {

GLProgram::GLProgram() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    m_handle = glCreateProgram();
    glProgramParameteri(m_handle, GL_PROGRAM_SEPARABLE, GL_TRUE);
}

GLProgram::~GLProgram() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    glDeleteProgram(m_handle);
}

uint32_t GLProgram::getHandle() const {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    return m_handle;
}

uint32_t GLProgram::attachShader(const Shader *shader) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    const auto *glShader = downcast<GLShader>(*shader);
    glAttachShader(m_handle, glShader->getHandle());
    m_attachedShaderHandles.push_back(glShader->getHandle());
    return glShader->getStageBit();
}

void GLProgram::link() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    glLinkProgram(m_handle);
    auto result = checkLinkStatus();
    NOX_ASSERT(!result);

    detachShaders();
}

void GLProgram::detachShaders() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    for (const auto shaderHandle : m_attachedShaderHandles) {
        glDetachShader(m_handle, shaderHandle);
    }
    m_attachedShaderHandles.clear();
}

bool GLProgram::checkLinkStatus() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    GLint result = GL_TRUE;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &result);

    if (result == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);

        std::string log;
        log.resize(length);
        glGetProgramInfoLog(m_handle, length, nullptr, log.data());

        NOX_LOG_ERROR(OPENGL, "GLSL shader program linking error\n{}", log);
        detachShaders();
        glDeleteProgram(m_handle);
    }

    return result;
}

} // namespace NOX
