#include "format_description.h"
#include "nox_assert.h"
#include "opengl/gl_helper.h"
#include "opengl/gl_vertex_array.h"

#include <glad/gl.h>

#include <algorithm>

namespace nox {

GLVertexArray::GLVertexArray(const VertexAttributes &vertexAttributes) {
    glCreateVertexArrays(1, &m_handle);

    for (size_t i = 0u; i < vertexAttributes.size(); i++) {
        const auto &vertexAttribute = vertexAttributes[i];
        auto description = getVertexAttributeFormatDescription(vertexAttribute);
        auto index = static_cast<GLuint>(i);
        auto componentsCount = static_cast<GLint>(description.componentsCount);
        auto dataType = mapVertexAttributeDataTypeToEnum(description.vertexAttributeDataType);
        auto normalized = static_cast<GLboolean>(description.isNormalized);
        auto offset = m_stride;

        glEnableVertexArrayAttrib(m_handle, index);
        glVertexArrayAttribFormat(m_handle, index, componentsCount, dataType, normalized, offset);
        glVertexArrayAttribBinding(m_handle, index, m_currentBindingIndex);

        m_stride += static_cast<GLuint>(description.stride);
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

void GLVertexArray::setIndexBuffer(uint32_t indexBufferHandle) const {
    glVertexArrayElementBuffer(m_handle, indexBufferHandle);
}

void GLVertexArray::bind() const {
    glBindVertexArray(m_handle);
}

void GLVertexArrayRegistry::registerVertexArray(const VertexAttributes &vertexAttributes) {
    m_vertexArraysEntries.emplace_back(vertexAttributes, std::make_unique<GLVertexArray>(vertexAttributes));
}

void GLVertexArrayRegistry::setBoundVertexArrayIndex(uint32_t index) {
    m_boundVertexArrayIndex = index;
}

uint32_t GLVertexArrayRegistry::getBoundVertexArrayIndex() const {
    return m_boundVertexArrayIndex;
}

GLVertexArray &GLVertexArrayRegistry::operator[](uint32_t index) {
    return *m_vertexArraysEntries[index].second;
}

const GLVertexArray &GLVertexArrayRegistry::operator[](uint32_t index) const {
    return *m_vertexArraysEntries[index].second;
}

bool GLVertexArrayRegistry::contains(const VertexAttributes &vertexAttributes) const {
    return (find(vertexAttributes) < static_cast<uint32_t>(m_vertexArraysEntries.size()));
}

uint32_t GLVertexArrayRegistry::find(const VertexAttributes &vertexAttributes) const {
    auto it = std::find_if(m_vertexArraysEntries.begin(),
                           m_vertexArraysEntries.end(),
                           [&vertexAttributes](const VertexArrayEntry &other) {
                               return (other.first == vertexAttributes);
                           });
    return static_cast<uint32_t>(std::distance(m_vertexArraysEntries.begin(), it));
}

} // namespace nox
