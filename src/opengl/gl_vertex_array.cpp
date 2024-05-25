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

GLVertexArray::GLVertexArray(VertexAttributes vertexAttributes)
    : m_vertexAttributes{std::move(vertexAttributes)} {
    glCreateVertexArrays(1, &m_handle);

    for (size_t i = 0u; i < m_vertexAttributes.size(); i++) {
        const auto &vertexAttribute = m_vertexAttributes[i];
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

GLVertexArrayRegistry &GLVertexArrayRegistry::instance() {
    static GLVertexArrayRegistry registry{};
    return registry;
}

GLVertexArrayRegistry::GLVertexArrayRegistry() {
    m_vertexArrays.emplace_back(GLVertexArray({}), 0u);
}

GLVertexArrayRegistry::~GLVertexArrayRegistry() {
    NOX_ASSERT(m_vertexArrays.size() == 1u);

    erase(0u);
}

uint32_t GLVertexArrayRegistry::registerVertexArray(const VertexAttributes &vertexAttributes) {
    if (!contains(vertexAttributes)) {
        m_vertexArrays.emplace_back(vertexAttributes, 0u);
    }

    auto index = find(vertexAttributes);
    auto &[vertexArray, references] = m_vertexArrays[index];
    references++;

    return index;
}

void GLVertexArrayRegistry::unregisterVertexArray(uint32_t index) {
    NOX_ASSERT(index < static_cast<uint32_t>(m_vertexArrays.size()));

    auto &[vertexArray, references] = m_vertexArrays[index];
    references--;
    if (references == 0u) {
        erase(index);
    }
}

void GLVertexArrayRegistry::setBoundVertexArrayIndex(uint32_t index) {
    m_boundVertexArrayIndex = index;
}

uint32_t GLVertexArrayRegistry::getBoundVertexArrayIndex() const {
    return m_boundVertexArrayIndex;
}

GLVertexArray &GLVertexArrayRegistry::getVertexArray(uint32_t index) {
    NOX_ASSERT(index < static_cast<uint32_t>(m_vertexArrays.size()));

    auto &[vertexArray, references] = m_vertexArrays[index];
    return vertexArray;
}

const GLVertexArray &GLVertexArrayRegistry::getVertexArray(uint32_t index) const {
    NOX_ASSERT(index < static_cast<uint32_t>(m_vertexArrays.size()));

    const auto &[vertexArray, references] = m_vertexArrays[index];
    return vertexArray;
}

bool GLVertexArrayRegistry::contains(const VertexAttributes &vertexAttributes) const {
    return (find(vertexAttributes) < static_cast<uint32_t>(m_vertexArrays.size()));
}

uint32_t GLVertexArrayRegistry::find(const VertexAttributes &vertexAttributes) const {
    for (size_t i = 0u; i < m_vertexArrays.size(); i++) {
        const auto &[vertexArray, references] = m_vertexArrays[i];
        if (vertexArray.getVertexAttributes() == vertexAttributes) {
            return (static_cast<uint32_t>(i));
        }
    }

    return (static_cast<uint32_t>(m_vertexArrays.size()));
}

void GLVertexArrayRegistry::erase(uint32_t index) {
    const auto &[vertexArray, references] = m_vertexArrays[index];
    m_vertexArrays.erase(m_vertexArrays.begin() + index);

    auto handle = vertexArray.getHandle();
    glDeleteVertexArrays(1, &handle);
}

} // namespace nox
