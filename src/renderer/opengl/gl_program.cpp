#include "renderer/opengl/gl_helper.h"
#include "renderer/opengl/gl_program.h"
#include "renderer/opengl/gl_shader.h"

namespace NOX {

GLProgram::GLProgram() {
    m_handle = glCreateProgram();
    glProgramParameteri(m_handle, GL_PROGRAM_SEPARABLE, GL_TRUE);
}

GLProgram::~GLProgram() {
    glDeleteProgram(m_handle);
}

uint32_t GLProgram::attachShader(const Shader *shader) {
    const auto *glShader = static_cast<const GLShader *>(shader);
    if (glShader != nullptr) {
        glAttachShader(m_handle, glShader->getHandle());
        m_attachedShaderHandles.push_back(glShader->getHandle());
        return GLHelper::mapShaderBit(glShader->getType());
    }

    return 0u;
}

void GLProgram::link() {
    glLinkProgram(m_handle);

    auto result = checkLinkStatus();
    NOX_ASSERT(!result);

    detachShaders();
}

void GLProgram::detachShaders() {
    for (const auto shaderHandle : m_attachedShaderHandles) {
        glDetachShader(m_handle, shaderHandle);
    }
    m_attachedShaderHandles.clear();
}

bool GLProgram::checkLinkStatus() {
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
