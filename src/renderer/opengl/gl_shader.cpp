#include "renderer/opengl/gl_shader.h"

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

    default: return GL_NONE;
    }
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

ShaderHandle GLShaderRegistry::registerShader(const ShaderDescriptor &descriptor, std::string_view source) {
    if (descriptor.name.empty() || source.empty()) {
        return {};
    }

    auto shader = std::make_unique<GLShader>(descriptor);
    if (!shader->compile(source.data())) {
        return {};
    }

    ShaderHandle handle{std::hash<std::string>{}(descriptor.name), true};
    m_shaders[handle.id] = std::move(shader);

    return handle;
}

void GLShaderRegistry::unregisterShader(ShaderHandle &handle) {
    if (contains(handle)) {
        m_shaders.erase(handle.id);
        handle.id = 0u;
        handle.isValid = false;
    }
}

bool GLShaderRegistry::contains(const ShaderHandle &handle) const {
    if (!handle.isValid) {
        return false;
    }
    return (m_shaders.find(handle.id) != m_shaders.end());
}

Shader &GLShaderRegistry::operator[](const ShaderHandle &handle) {
    return getShader(handle);
}

const Shader &GLShaderRegistry::operator[](const ShaderHandle &handle) const {
    return getShader(handle);
}

GLShader &GLShaderRegistry::getShader(const ShaderHandle &handle) {
    return *m_shaders.at(handle.id);
}

const GLShader &GLShaderRegistry::getShader(const ShaderHandle &handle) const {
    return *m_shaders.at(handle.id);
}

} // namespace NOX
