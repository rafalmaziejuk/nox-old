#include "renderer/format_helper.h"
#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_helper.h"

namespace NOX {

GLBuffer::GLBuffer(const BufferDescriptor &descriptor) {
    auto flags = GLHelper::mapBufferStorageFlags(descriptor.accessMethod);
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, descriptor.size, descriptor.data, flags);
}

GLBuffer::~GLBuffer() {
    glDeleteBuffers(1, &m_handle);
}

void GLVertexBuffer::setVertexArrayIndex(uint32_t index) {
    m_vertexArrayIndex = index;
}

uint32_t GLVertexBuffer::getVertexArrayIndex() const {
    return m_vertexArrayIndex;
}

void GLIndexBuffer::setIndexType(Format format) {
    auto formatDescriptor = FormatHelper::getFormatDescriptor(format);
    m_indexType = GLHelper::mapDataTypeEnum(formatDescriptor.dataType, formatDescriptor.dataTypeSize);
}

uint32_t GLIndexBuffer::getIndexType() const {
    return m_indexType;
}

} // namespace NOX
