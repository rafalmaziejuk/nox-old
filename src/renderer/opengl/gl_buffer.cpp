#include "renderer/format_helper.h"
#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_helper.h"

namespace NOX {

namespace {

GLbitfield mapBufferUsageToBitfield(uint32_t usage) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    GLbitfield flags = 0u;

    if (usage & BufferUsage::STATIC) {
        flags |= 0u;
    }
    if (usage & BufferUsage::DYNAMIC) {
        flags |= GL_DYNAMIC_STORAGE_BIT;
    }

    return flags;
}

} // namespace

GLBuffer::GLBuffer(const BufferDescriptor &descriptor) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    auto flags = mapBufferUsageToBitfield(descriptor.usage);
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, descriptor.size, descriptor.data, flags);
}

GLBuffer::~GLBuffer() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    glDeleteBuffers(1, &m_handle);
}

uint32_t GLBuffer::getHandle() const {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    return m_handle;
}

void GLVertexBuffer::setVertexArrayIndex(uint32_t index) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    m_vertexArrayIndex = index;
}

uint32_t GLVertexBuffer::getVertexArrayIndex() const {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    return m_vertexArrayIndex;
}

void GLIndexBuffer::setIndexType(Format format) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    auto formatDescriptor = FormatHelper::getFormatDescriptor(format);
    m_indexType = GLHelper::mapFormatDataTypeToEnum(formatDescriptor.dataType, formatDescriptor.dataTypeSize);
}

uint32_t GLIndexBuffer::getIndexType() const {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    return m_indexType;
}

} // namespace NOX
