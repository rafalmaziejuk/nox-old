#include "format_helper.h"
#include "opengl/gl_helper.h"
#include "opengl/gl_vertex_array.h"

#include <algorithm>

namespace NOX {

GLVertexArray::GLVertexArray(const VertexFormat &vertexFormat) {
    glCreateVertexArrays(1, &m_handle);

    for (size_t i = 0u; i < vertexFormat.size(); i++) {
        const auto &format = vertexFormat[i];
        auto formatDescriptor = Helpers::getFormatDescriptor(format);
        auto index = static_cast<GLuint>(i);
        auto componentCount = static_cast<GLint>(formatDescriptor.componentCount);
        auto dataType = GLHelper::mapFormatDataTypeToEnum(formatDescriptor.dataType, formatDescriptor.dataTypeSize);
        auto normalized = static_cast<GLboolean>(formatDescriptor.isNormalized);
        auto offset = m_stride;
        m_stride += static_cast<GLuint>(formatDescriptor.componentCount * formatDescriptor.dataTypeSize);

        glEnableVertexArrayAttrib(m_handle, index);
        glVertexArrayAttribFormat(m_handle, index, componentCount, dataType, normalized, offset);
        glVertexArrayAttribBinding(m_handle, index, m_currentBindingIndex);
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

uint32_t GLVertexArrayRegistry::registerVertexArray(const VertexFormat &vertexFormat) {
    auto index = find(vertexFormat);
    if (index == invalidVertexArrayIndex) {
        index = static_cast<uint32_t>(m_vertexArrays.size());
        m_vertexArrays.emplace_back(std::make_unique<GLVertexArray>(vertexFormat));
        m_vertexFormats.push_back(vertexFormat);
    }

    return index;
}

GLVertexArray &GLVertexArrayRegistry::operator[](uint32_t index) {
    return *m_vertexArrays[index];
}

const GLVertexArray &GLVertexArrayRegistry::operator[](uint32_t index) const {
    return *m_vertexArrays[index];
}

void GLVertexArrayRegistry::setBoundVertexArrayIndex(uint32_t index) {
    m_boundVertexArrayIndex = index;
}

uint32_t GLVertexArrayRegistry::getBoundVertexArrayIndex() const {
    return m_boundVertexArrayIndex;
}

uint32_t GLVertexArrayRegistry::find(const VertexFormat &vertexFormat) const {
    auto it = std::find_if(m_vertexFormats.begin(), m_vertexFormats.end(), [&vertexFormat](const VertexFormat &format) {
        return (format == vertexFormat);
    });
    if (it != m_vertexFormats.end()) {
        return static_cast<uint32_t>(std::distance(m_vertexFormats.begin(), it));
    }
    return invalidVertexArrayIndex;
}

} // namespace NOX
