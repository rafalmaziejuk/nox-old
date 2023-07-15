#include "renderer/format_helper.h"
#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_helper.h"
#include "renderer/opengl/gl_vertex_array.h"

namespace NOX {

namespace {

struct ProcessedVertexAttribute {
    GLuint index;
    GLint componentCount;
    GLenum dataType;
    GLboolean normalized;
    GLuint offset;
};

struct VertexArrayDescriptor {
    explicit VertexArrayDescriptor(const VertexFormat &format) {
        const auto &attributes = format.attributes;
        auto attributesCount = attributes.size();
        processedAttributes.resize(attributesCount);
        for (size_t i = 0; i < attributesCount; i++) {
            auto formatDescriptor = FormatHelper::getFormatDescriptor(attributes[i].format);
            auto &attribute = processedAttributes[i];
            attribute.index = static_cast<GLuint>(i);
            attribute.componentCount = static_cast<GLint>(formatDescriptor.componentCount);
            attribute.dataType = GLHelper::mapFormatDataTypeToEnum(formatDescriptor.dataType, formatDescriptor.dataTypeSize);
            attribute.normalized = static_cast<GLboolean>(formatDescriptor.isNormalized);
            attribute.offset = stride;

            stride += static_cast<GLuint>(formatDescriptor.componentCount * formatDescriptor.dataTypeSize);
        }
    }

    std::vector<ProcessedVertexAttribute> processedAttributes;
    uint32_t stride{0u};
};

} // namespace

GLVertexArray::GLVertexArray() {
    glCreateVertexArrays(1, &m_handle);
}

GLVertexArray::~GLVertexArray() {
    glDeleteVertexArrays(1, &m_handle);
}

void GLVertexArray::setVertexBuffer(const GLBuffer &vertexBuffer, const VertexFormat &format) {
    constexpr GLuint offsetToFirstElementInsideBuffer = 0u;
    const VertexArrayDescriptor vertexArrayDescriptor{format};

    glVertexArrayVertexBuffer(m_handle, m_currentBindingIndex, vertexBuffer.getHandle(), offsetToFirstElementInsideBuffer, vertexArrayDescriptor.stride);
    for (const auto &attribute : vertexArrayDescriptor.processedAttributes) {
        glEnableVertexArrayAttrib(m_handle, attribute.index);
        glVertexArrayAttribFormat(m_handle, attribute.index, attribute.componentCount, attribute.dataType, attribute.normalized, attribute.offset);
        glVertexArrayAttribBinding(m_handle, attribute.index, m_currentBindingIndex);
    }

    m_currentBindingIndex++;
}

void GLVertexArray::setIndexBuffer(const GLBuffer &indexBuffer) const {
    glVertexArrayElementBuffer(m_handle, indexBuffer.getHandle());
}

void GLVertexArray::bind() const {
    glBindVertexArray(m_handle);
}

} // namespace NOX
