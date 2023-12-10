#include "format_helper.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_helper.h"

#include <algorithm>

namespace NOX {

namespace {

GLbitfield mapBufferUsageToBitfield(uint32_t usage) {
    GLbitfield flags = GL_NONE;

    if (usage & BufferUsage::STATIC) {
        flags |= 0u;
    }
    if (usage & BufferUsage::DYNAMIC) {
        flags |= GL_DYNAMIC_STORAGE_BIT;
    }

    return flags;
}

} // namespace

bool GLBuffer::validateInput(const BufferDescriptor &descriptor) {
    return (descriptor.size > 0u) &&
           (descriptor.data != nullptr);
}

GLBuffer::GLBuffer(const BufferDescriptor &descriptor, GLState &state) : GLWithState{state} {
    auto flags = mapBufferUsageToBitfield(descriptor.usage);
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, descriptor.size, descriptor.data, flags);
}

GLBuffer::~GLBuffer() {
    glDeleteBuffers(1, &m_handle);
}

bool GLVertexBuffer::validateInput(const VertexBufferDescriptor &descriptor) {
    auto validateFormat = [](Format format) -> bool {
        auto formatDescriptor = Helpers::getFormatDescriptor(format);
        auto indexType = GLHelper::mapFormatDataTypeToEnum(formatDescriptor.dataType, formatDescriptor.dataTypeSize);
        return (indexType != GL_NONE);
    };

    return (GLBuffer::validateInput(descriptor)) &&
           (!descriptor.vertexFormat.empty()) &&
           (std::all_of(descriptor.vertexFormat.begin(), descriptor.vertexFormat.end(), validateFormat));
}

void GLVertexBuffer::bind() {
    auto &vertexArrayRegistry = getState().vertexArrayRegistry;
    vertexArrayRegistry[m_vertexArrayIndex].bind();
    vertexArrayRegistry.setBoundVertexArrayIndex(m_vertexArrayIndex);
}

void GLVertexBuffer::setVertexArrayIndex(uint32_t index) {
    m_vertexArrayIndex = index;
}

bool GLIndexBuffer::validateInput(const IndexBufferDescriptor &descriptor) {
    auto formatDescriptor = Helpers::getFormatDescriptor(descriptor.format);
    auto indexType = GLHelper::mapFormatDataTypeToEnum(formatDescriptor.dataType, formatDescriptor.dataTypeSize);

    return (GLBuffer::validateInput(descriptor)) &&
           (indexType != GL_NONE);
}

void GLIndexBuffer::bind() {
    auto &state = getState();
    state.indexType = m_indexType;

    auto &vertexArrayRegistry = state.vertexArrayRegistry;
    auto &vertexArray = vertexArrayRegistry[vertexArrayRegistry.getBoundVertexArrayIndex()];
    vertexArray.setIndexBuffer(getHandle());
}

void GLIndexBuffer::setIndexType(Format format) {
    auto formatDescriptor = Helpers::getFormatDescriptor(format);
    m_indexType = GLHelper::mapFormatDataTypeToEnum(formatDescriptor.dataType, formatDescriptor.dataTypeSize);
}

} // namespace NOX
