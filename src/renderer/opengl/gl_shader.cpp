#include "renderer/opengl/gl_shader.h"

#include <glad/gl.h>

namespace NOX {

namespace {

const char *mapShaderStageBitToString(uint32_t stageBit) {
    switch (stageBit) {
        NOX_CASE_RETURN_STRING(GL_VERTEX_SHADER_BIT, GL_VERTEX_SHADER);
        NOX_CASE_RETURN_STRING(GL_FRAGMENT_SHADER_BIT, GL_FRAGMENT_SHADER);

    default:
        NOX_ASSERT(true);
        return nullptr;
    }
}

GLenum mapShaderStageToEnum(uint32_t stage) {
    switch (stage) {
    case ShaderStage::VERTEX:
        return GL_VERTEX_SHADER;
    case ShaderStage::FRAGMENT:
        return GL_FRAGMENT_SHADER;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

GLbitfield mapShaderStageToBitfield(uint32_t stage) {
    switch (stage) {
    case ShaderStage::VERTEX:
        return GL_VERTEX_SHADER_BIT;
    case ShaderStage::FRAGMENT:
        return GL_FRAGMENT_SHADER_BIT;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

} // namespace

GLShader::GLShader(const ShaderDescriptor &descriptor) {
    m_handle = glCreateShader(mapShaderStageToEnum(descriptor.stage));
    m_stageBit = mapShaderStageToBitfield(descriptor.stage);
}

GLShader::~GLShader() {
    glDeleteShader(m_handle);
}

void GLShader::compile(const char *source) const {
    glShaderSource(m_handle, 1, &source, nullptr);
    glCompileShader(m_handle);

    GLint result = GL_TRUE;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);

        std::string log;
        log.resize(length);
        glGetShaderInfoLog(m_handle, length, nullptr, log.data());

        glDeleteShader(m_handle);

        NOX_ASSERT(true);
    }
}

void GLShader::compileFromString(std::string_view source) const {
    compile(source.data());
}

} // namespace NOX
