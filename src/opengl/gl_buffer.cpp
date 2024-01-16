#include "format_description.h"
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

bool GLBuffer::validateInput(const BufferDescription &description) {
    bool result = true;

    result &= (description.size > 0u);
    result &= (description.data != nullptr);

    return result;
}

GLBuffer::GLBuffer(const BufferDescription &description, GLState &state) : GLWithState{state} {
    auto flags = mapBufferUsageToBitfield(description.usage);
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, description.size, description.data, flags);
}

GLBuffer::~GLBuffer() {
    glDeleteBuffers(1, &m_handle);
}

bool GLVertexBuffer::validateInput(const VertexBufferDescription &description) {
    bool result = true;

    result &= GLBuffer::validateInput(description);
    result &= (!description.vertexAttributes.empty());

    return result;
}

void GLVertexBuffer::bind() {
    auto &vertexArrayRegistry = getState().vertexArrayRegistry;
    vertexArrayRegistry[m_vertexArrayIndex].bind();
    vertexArrayRegistry.setBoundVertexArrayIndex(m_vertexArrayIndex);
}

void GLVertexBuffer::setVertexArrayIndex(uint32_t index) {
    m_vertexArrayIndex = index;
}

bool GLIndexBuffer::validateInput(const IndexBufferDescription &description) {
    bool result = true;

    result &= GLBuffer::validateInput(description);

    return result;
}

void GLIndexBuffer::bind() {
    auto &state = getState();
    state.indexType = m_indexType;

    auto &vertexArrayRegistry = state.vertexArrayRegistry;
    auto &vertexArray = vertexArrayRegistry[vertexArrayRegistry.getBoundVertexArrayIndex()];
    vertexArray.setIndexBuffer(getHandle());
}

void GLIndexBuffer::setIndexType(VertexAttributeFormat format) {
    auto description = getVertexAttributeFormatDescription(format);
    m_indexType = mapVertexAttributeDataTypeToEnum(description.vertexAttributeDataType);
}

} // namespace nox
