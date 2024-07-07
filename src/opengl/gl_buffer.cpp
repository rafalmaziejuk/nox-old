#include "asserts.h"
#include "format_descriptor.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_vertex_array.h"

#include <glad/gl.h>

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

GLenum mapVertexAttributeTypeToEnum(VertexAttributeType type) {
    switch (type) {
    case VertexAttributeType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
    case VertexAttributeType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
    case VertexAttributeType::UNSIGNED_INT: return GL_UNSIGNED_INT;

    default: break;
    }

    return GL_NONE;
}

} // namespace

GLBuffer::GLBuffer(const BufferDescriptor &descriptor) {
    auto flags = mapBufferUsageToBitfield(descriptor.usage);
    glCreateBuffers(1, &m_handle);

    constexpr auto immutableStorageBuffer = true;
    if constexpr (immutableStorageBuffer) {
        allocateImmutableStorage(descriptor.size, descriptor.data, flags);
    }
}

GLBuffer::~GLBuffer() {
    glDeleteBuffers(1, &m_handle);
}

void GLBuffer::allocateImmutableStorage(uint32_t size, const void *data, uint32_t flags) const {
    glNamedBufferStorage(m_handle, size, data, flags);
}

std::unique_ptr<GLVertexBuffer> GLVertexBuffer::create(const VertexBufferDescriptor &descriptor,
                                                       std::shared_ptr<GLVertexArrayRegistry> registry) {
    auto buffer = std::make_unique<GLVertexBuffer>(descriptor, std::move(registry));
    NOX_ENSURE_RETURN_NULLPTR(buffer->getHandle() != 0u);

    return buffer;
}

GLVertexBuffer::GLVertexBuffer(const VertexBufferDescriptor &descriptor,
                               std::shared_ptr<GLVertexArrayRegistry> registry)
    : GLBuffer{descriptor},
      m_vertexArrayRegistry{std::move(registry)},
      m_vertexAttributes{descriptor.vertexAttributes} {
    m_vertexArray = m_vertexArrayRegistry->registerVertexArray(m_vertexAttributes);
    m_vertexArray->setVertexBuffer(getHandle());
}

GLVertexBuffer::~GLVertexBuffer() {
    m_vertexArrayRegistry->unregisterVertexArray(m_vertexAttributes);
}

void GLVertexBuffer::bind() const {
    m_vertexArray->bind();
    m_vertexArrayRegistry->setBoundVertexArray(m_vertexAttributes);
}

std::unique_ptr<GLIndexBuffer> GLIndexBuffer::create(const IndexBufferDescriptor &descriptor,
                                                     std::shared_ptr<GLVertexArrayRegistry> registry) {
    auto buffer = std::make_unique<GLIndexBuffer>(descriptor, std::move(registry));
    NOX_ENSURE_RETURN_NULLPTR(buffer->getHandle() != 0u);

    return buffer;
}

GLIndexBuffer::GLIndexBuffer(const IndexBufferDescriptor &descriptor,
                             std::shared_ptr<GLVertexArrayRegistry> registry)
    : GLBuffer{descriptor},
      m_vertexArrayRegistry{std::move(registry)} {
    auto formatDescriptor = getVertexAttributeFormatDescriptor(descriptor.format);
    m_indexType = mapVertexAttributeTypeToEnum(formatDescriptor.vertexAttributeType);
}

void GLIndexBuffer::bind() const {
    auto vertexArray = m_vertexArrayRegistry->getBoundVertexArray();
    vertexArray->setIndexBuffer(getHandle());
}

} // namespace nox
