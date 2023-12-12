#include "format_descriptor.h"
#include "nox_assert.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_helper.h"

#include <glad/gl.h>

#include <algorithm>

namespace nox {

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
    return (GLBuffer::validateInput(descriptor)) &&
           (!descriptor.vertexAttributes.empty());
}

void GLVertexBuffer::bind() {
    auto &state = getState();
    auto &vertexArrayRegistry = getState().vertexArrayRegistry;
    vertexArrayRegistry[m_vertexArrayIndex].bind();
    state.boundVertexArrayIndex = m_vertexArrayIndex;
}

void GLVertexBuffer::setVertexArrayIndex(uint32_t index) {
    m_vertexArrayIndex = index;
}

bool GLIndexBuffer::validateInput(const IndexBufferDescriptor &descriptor) {
    return GLBuffer::validateInput(descriptor);
}

void GLIndexBuffer::bind() {
    auto &state = getState();
    state.indexType = m_indexType;

    auto &vertexArrayRegistry = state.vertexArrayRegistry;
    auto &vertexArray = vertexArrayRegistry[state.boundVertexArrayIndex];
    vertexArray.setIndexBuffer(getHandle());
}

void GLIndexBuffer::setIndexType(VertexAttributeFormat format) {
    auto descriptor = getVertexAttributeFormatDescriptor(format);
    m_indexType = mapVertexAttributeDataTypeToEnum(descriptor.vertexAttributeDataType);
}

} // namespace nox
