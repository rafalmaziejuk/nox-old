#pragma once

#include <nox/buffer.h>

#include <map>
#include <memory>

namespace nox {

class GLVertexArray final {
  public:
    explicit GLVertexArray(const VertexAttributes &vertexAttributes);
    ~GLVertexArray();

    [[nodiscard]] uint32_t getHandle() const {
        return m_handle;
    }

    void setVertexBuffer(uint32_t vertexBufferHandle);
    void setIndexBuffer(uint32_t indexBufferHandle);

    void bind() const;

  private:
    uint32_t m_currentBindingIndex{0u};
    uint32_t m_stride{0u};
    uint32_t m_handle{0u};
};

class GLVertexArrayRegistry final {
  public:
    [[nodiscard]] static std::shared_ptr<GLVertexArrayRegistry> create();

    [[nodiscard]] std::shared_ptr<GLVertexArray> registerVertexArray(const VertexAttributes &vertexAttributes);
    void unregisterVertexArray(const VertexAttributes &vertexAttributes);

    void setBoundVertexArray(const VertexAttributes &vertexAttributes);
    [[nodiscard]] std::shared_ptr<GLVertexArray> getBoundVertexArray() const;

  private:
    [[nodiscard]] bool contains(const VertexAttributes &vertexAttributes) const;

  public:
    GLVertexArrayRegistry() = default;
    GLVertexArrayRegistry(const GLVertexArrayRegistry &) = delete;
    GLVertexArrayRegistry &operator=(const GLVertexArrayRegistry &) = delete;
    GLVertexArrayRegistry(GLVertexArrayRegistry &&) = delete;
    GLVertexArrayRegistry &operator=(GLVertexArrayRegistry &&) = delete;

  private:
    std::map<VertexAttributes, std::shared_ptr<GLVertexArray>> m_vertexArrays;
    VertexAttributes m_boundVertexArrayKey;
};

} // namespace nox
