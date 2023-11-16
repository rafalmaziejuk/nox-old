#include "renderer/opengl/gl_shader.h"
#include "renderer/opengl/gl_shader_visitor.h"

#include <nox/config.h>

#include <glad/gl.h>

namespace NOX {

namespace {

GLenum mapShaderTypeToEnum(ShaderType type) {
    switch (type) {
    case ShaderType::VERTEX: return GL_VERTEX_SHADER;
    case ShaderType::FRAGMENT: return GL_FRAGMENT_SHADER;
    case ShaderType::TESS_CONTROL: return GL_TESS_CONTROL_SHADER;
    case ShaderType::TESS_EVALUATION: return GL_TESS_EVALUATION_SHADER;
    case ShaderType::GEOMETRY: return GL_GEOMETRY_SHADER;
    case ShaderType::COMPUTE: return GL_COMPUTE_SHADER;

    default: break;
    }

    NOX_ASSERT(false);
    return GL_NONE;
}

} // namespace

GLShader::GLShader(const ShaderDescriptor &descriptor) : m_type{descriptor.type} {
    m_handle = glCreateShader(mapShaderTypeToEnum(m_type));
}

GLShader::~GLShader() {
    glDeleteShader(m_handle);
}

bool GLShader::compile(const char *source) const {
    glShaderSource(m_handle, 1, &source, nullptr);
    glCompileShader(m_handle);

    GLint result = GL_TRUE;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        glDeleteShader(m_handle);
        return false;
    }

    return true;
}

void GLShader::accept(ShaderVisitor &visitor) const {
    visitor.visit(*this);
}

} // namespace NOX
