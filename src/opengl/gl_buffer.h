#pragma once

#include "opengl/gl_state.h"

#include <nox/buffer.h>

namespace nox {

class GLBuffer : public Buffer {
  public:
    explicit GLBuffer(const BufferDescriptor &descriptor);
    ~GLBuffer() override;

    [[nodiscard]] uint32_t getHandle() const { return m_handle; }

  protected:
    [[nodiscard]] static bool validateInput(const BufferDescriptor &descriptor);

  private:
    uint32_t m_handle{0u};
};

class GLVertexBuffer final : public GLBuffer {
  public:
    [[nodiscard]] static bool validateInput(const VertexBufferDescriptor &descriptor);

    using GLBuffer::GLBuffer;
    ~GLVertexBuffer() override;

    void setVertexArrayIndex(uint32_t index);
    [[nodiscard]] uint32_t getVertexArrayIndex() const {
        return m_vertexArrayIndex;
    }

  private:
    uint32_t m_vertexArrayIndex{0u};
};

class GLIndexBuffer final : public GLBuffer {
  public:
    [[nodiscard]] static bool validateInput(const IndexBufferDescriptor &descriptor);

    using GLBuffer::GLBuffer;

    void setIndexType(VertexAttributeFormat format);
    [[nodiscard]] uint32_t getIndexType() const {
        return m_indexType;
    }

  private:
    uint32_t m_indexType{0u};
};

} // namespace nox
