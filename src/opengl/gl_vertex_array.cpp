#include "asserts.h"
#include "format_descriptor.h"
#include "opengl/gl_vertex_array.h"

#include <glad/gl.h>

namespace nox {

namespace {

GLenum mapVertexAttributeTypeToEnum(VertexAttributeType type) {
    switch (type) {
    case VertexAttributeType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
    case VertexAttributeType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
    case VertexAttributeType::UNSIGNED_INT: return GL_UNSIGNED_INT;
    case VertexAttributeType::BYTE: return GL_BYTE;
    case VertexAttributeType::SHORT: return GL_SHORT;
    case VertexAttributeType::INT: return GL_INT;
    case VertexAttributeType::HALF_FLOAT: return GL_HALF_FLOAT;
    case VertexAttributeType::FLOAT: return GL_FLOAT;
    default: break;
    }

    NOX_ASSERT(false);
    return GL_NONE;
}

} // namespace

GLVertexArray::GLVertexArray(const VertexAttributes &vertexAttributes) {
    glCreateVertexArrays(1, &m_handle);

    for (size_t i = 0u; i < vertexAttributes.size(); i++) {
        const auto &vertexAttribute = vertexAttributes[i];
        auto descriptor = getVertexAttributeFormatDescriptor(vertexAttribute);
        auto index = static_cast<GLuint>(i);
        auto componentsCount = static_cast<GLint>(descriptor.componentsCount);
        auto dataType = mapVertexAttributeTypeToEnum(descriptor.vertexAttributeType);
        auto normalized = static_cast<GLboolean>(descriptor.isNormalized);
        auto offset = m_stride;

        glEnableVertexArrayAttrib(m_handle, index);
        glVertexArrayAttribFormat(m_handle, index, componentsCount, dataType, normalized, offset);
        glVertexArrayAttribBinding(m_handle, index, m_currentBindingIndex);

        m_stride += static_cast<GLuint>(descriptor.stride);
    }
}

GLVertexArray::~GLVertexArray() {
    glDeleteVertexArrays(1, &m_handle);
}

void GLVertexArray::setVertexBuffer(uint32_t vertexBufferHandle) {
    constexpr GLintptr offsetToFirstElementInsideBuffer = 0;
    glVertexArrayVertexBuffer(m_handle,
                              m_currentBindingIndex,
                              vertexBufferHandle,
                              offsetToFirstElementInsideBuffer,
                              static_cast<GLsizei>(m_stride));
    m_currentBindingIndex++;
}

void GLVertexArray::setIndexBuffer(uint32_t indexBufferHandle) { // NOLINT(readability-make-member-function-const)
    glVertexArrayElementBuffer(m_handle, indexBufferHandle);
}

void GLVertexArray::bind() const {
    glBindVertexArray(m_handle);
}

std::shared_ptr<GLVertexArrayRegistry> GLVertexArrayRegistry::create() {
    return std::make_shared<GLVertexArrayRegistry>();
}

std::shared_ptr<GLVertexArray> GLVertexArrayRegistry::registerVertexArray(const VertexAttributes &vertexAttributes) {
    if (!contains(vertexAttributes)) {
        auto vertexArray = std::make_shared<GLVertexArray>(vertexAttributes);
        m_vertexArrays.emplace(vertexAttributes, std::move(vertexArray));
    }

    return m_vertexArrays[vertexAttributes];
}

void GLVertexArrayRegistry::unregisterVertexArray(const VertexAttributes &vertexAttributes) {
    NOX_ASSERT(contains(vertexAttributes));

    m_vertexArrays.erase(vertexAttributes);
}

void GLVertexArrayRegistry::setBoundVertexArray(const VertexAttributes &vertexAttributes) {
    m_boundVertexArrayKey = vertexAttributes;
}

std::shared_ptr<GLVertexArray> GLVertexArrayRegistry::getBoundVertexArray() const {
    NOX_ASSERT(contains(m_boundVertexArrayKey));

    return m_vertexArrays.at(m_boundVertexArrayKey);
}

bool GLVertexArrayRegistry::contains(const VertexAttributes &vertexAttributes) const {
    auto it = m_vertexArrays.find(vertexAttributes);
    return (it != m_vertexArrays.end());
}

} // namespace nox
