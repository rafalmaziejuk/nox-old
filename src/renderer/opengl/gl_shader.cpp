#include "renderer/opengl/gl_shader.h"

#include <glad/gl.h>

namespace NOX {

namespace {

const char *mapShaderStageBitToString(uint32_t stageBit) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    switch (stageBit) {
        NOX_CASE_RETURN_STRING(GL_VERTEX_SHADER_BIT, GL_VERTEX_SHADER);
        NOX_CASE_RETURN_STRING(GL_FRAGMENT_SHADER_BIT, GL_FRAGMENT_SHADER);

    default:
        NOX_ASSERT(true);
        return nullptr;
    }
}

GLenum mapShaderStageToEnum(uint32_t stage) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

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
    NOX_LOG_TRACE_DECLARE(OPENGL);

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
    NOX_LOG_TRACE_DECLARE(OPENGL);

    m_handle = glCreateShader(mapShaderStageToEnum(descriptor.stage));
    m_stageBit = mapShaderStageToBitfield(descriptor.stage);
}

GLShader::~GLShader() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    glDeleteShader(m_handle);
}

uint32_t GLShader::getHandle() const {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    return m_handle;
}

uint32_t GLShader::getStageBit() const {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    return m_stageBit;
}

void GLShader::compile(const char *source) const {
    NOX_LOG_TRACE_DECLARE(OPENGL);

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

        NOX_LOG_ERROR(OPENGL, "{} compilation error\n{}", mapShaderStageBitToString(m_stageBit), log);
        glDeleteShader(m_handle);

        NOX_ASSERT(true);
    }
}

void GLShader::compileFromString(std::string_view source) const {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    compile(source.data());
}

} // namespace NOX
