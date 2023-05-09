#pragma once

#include "renderer/opengl/gl_object.h"

#include <nox/renderer/buffer.h>

namespace NOX {

class GLBuffer : public Buffer, public GLObject {
  public:
    explicit GLBuffer(const BufferDescriptor &descriptor);
    ~GLBuffer() override;
};

class GLVertexBuffer final : public GLBuffer {
  public:
    using GLBuffer::GLBuffer;

    void setVertexArrayIndex(uint32_t index);
    uint32_t getVertexArrayIndex() const;

  private:
    uint32_t m_vertexArrayIndex{0u};
};

class GLIndexBuffer final : public GLBuffer {
  public:
    using GLBuffer::GLBuffer;

    void setIndexType(Format format);
    uint32_t getIndexType() const;

  private:
    uint32_t m_indexType{0u};
};

} // namespace NOX
