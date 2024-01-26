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
    bool result = true;

    result &= (descriptor.size > 0u);
    result &= (descriptor.data != nullptr);

    return result;
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
    bool result = true;

    result &= GLBuffer::validateInput(descriptor);
    result &= (!descriptor.vertexAttributes.empty());

    return result;
}

void GLVertexBuffer::bind() {
    auto &vertexArrayRegistry = state->vertexArrayRegistry;
    vertexArrayRegistry[m_vertexArrayIndex].bind();
    vertexArrayRegistry.setBoundVertexArrayIndex(m_vertexArrayIndex);
}

void GLVertexBuffer::setVertexArrayIndex(uint32_t index) {
    m_vertexArrayIndex = index;
}

bool GLIndexBuffer::validateInput(const IndexBufferDescriptor &descriptor) {
    bool result = true;

    result &= GLBuffer::validateInput(descriptor);

    return result;
}

void GLIndexBuffer::bind() {
    state->indexType = m_indexType;

    auto &vertexArrayRegistry = state->vertexArrayRegistry;
    auto &vertexArray = vertexArrayRegistry[vertexArrayRegistry.getBoundVertexArrayIndex()];
    vertexArray.setIndexBuffer(getHandle());
}

void GLIndexBuffer::setIndexType(VertexAttributeFormat format) {
    auto descriptor = getVertexAttributeFormatDescriptor(format);
    m_indexType = mapVertexAttributeDataTypeToEnum(descriptor.vertexAttributeDataType);
}

} // namespace nox
