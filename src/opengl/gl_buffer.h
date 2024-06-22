#pragma once

#include <nox/buffer.h>

#include <memory>

namespace nox {

class GLVertexArray;
class GLVertexArrayRegistry;

class GLBuffer : public Buffer {
  public:
    explicit GLBuffer(const BufferDescriptor &descriptor);
    ~GLBuffer() override;

    [[nodiscard]] uint32_t getHandle() const { return m_handle; }

    virtual void bind() const = 0;

  private:
    void allocateImmutableStorage(uint32_t size, const void *data, uint32_t flags) const;

  private:
    uint32_t m_handle{0u};
};

class GLVertexBuffer final : public GLBuffer {
  public:
    [[nodiscard]] static std::unique_ptr<GLVertexBuffer> create(const VertexBufferDescriptor &descriptor,
                                                                std::shared_ptr<GLVertexArrayRegistry> registry);

    GLVertexBuffer(const VertexBufferDescriptor &descriptor,
                   std::shared_ptr<GLVertexArrayRegistry> registry);
    ~GLVertexBuffer() override;

    void bind() const override;

  private:
    std::shared_ptr<GLVertexArrayRegistry> m_vertexArrayRegistry{nullptr};
    std::shared_ptr<GLVertexArray> m_vertexArray{nullptr};
    VertexAttributes m_vertexAttributes;
};

class GLIndexBuffer final : public GLBuffer {
  public:
    [[nodiscard]] static std::unique_ptr<GLIndexBuffer> create(const IndexBufferDescriptor &descriptor,
                                                               std::shared_ptr<GLVertexArrayRegistry> registry);

    GLIndexBuffer(const IndexBufferDescriptor &descriptor,
                  std::shared_ptr<GLVertexArrayRegistry> registry);

    [[nodiscard]] uint32_t getIndexType() const {
        return m_indexType;
    }

    void bind() const override;

  private:
    std::shared_ptr<GLVertexArrayRegistry> m_vertexArrayRegistry{nullptr};
    uint32_t m_indexType{0u};
};

} // namespace nox
