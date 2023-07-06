#include "renderer/opengl/gl_helper.h"
#include "renderer/opengl/gl_shader.h"

namespace NOX {

namespace {

const char *shaderTypeToString(ShaderType type) {
    switch (type) {
    case ShaderType::VERTEX:
        return "vertex";
    case ShaderType::FRAGMENT:
        return "fragment";
    default:
        NOX_ASSERT(true);
        return "undefined";
    }
}

} // namespace

GLShader::GLShader(const ShaderDescriptor &descriptor) : m_type{descriptor.type} {
    m_handle = glCreateShader(GLHelper::mapShaderEnum(m_type));
}

GLShader::~GLShader() {
    glDeleteShader(m_handle);
}

void GLShader::compileFromString(std::string_view source) {
    compile(source.data());
}

void GLShader::compile(const char *source) {
    glShaderSource(m_handle, 1, &source, nullptr);
    glCompileShader(m_handle);

    auto result = checkCompileStatus(m_type);
    NOX_ASSERT(!result);
}

bool GLShader::checkCompileStatus(ShaderType type) const {
    GLint result = GL_TRUE;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);

        std::string log;
        log.resize(length);
        glGetShaderInfoLog(m_handle, length, nullptr, log.data());

        NOX_LOG_ERROR(OPENGL, "GLSL {} shader compilation error\n{}", shaderTypeToString(type), log);
        glDeleteShader(m_handle);
    }

    return result;
}

} // namespace NOX
