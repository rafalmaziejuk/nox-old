#pragma once

#include <nox/buffer.h>

#include <memory>
#include <vector>

namespace nox {

class GLVertexArray final {
  public:
    explicit GLVertexArray(VertexAttributes vertexAttributes);

    [[nodiscard]] const VertexAttributes &getVertexAttributes() const {
        return m_vertexAttributes;
    }

    [[nodiscard]] uint32_t getHandle() const {
        return m_handle;
    }

    void setVertexBuffer(uint32_t vertexBufferHandle);
    void setIndexBuffer(uint32_t indexBufferHandle);

    void bind() const;

  private:
    VertexAttributes m_vertexAttributes;
    uint32_t m_currentBindingIndex{0u};
    uint32_t m_stride{0u};
    uint32_t m_handle{0u};
};

class GLVertexArrayRegistry final {
  public:
    [[nodiscard]] static GLVertexArrayRegistry &instance();

    void initialize();

    [[nodiscard]] uint32_t registerVertexArray(const VertexAttributes &vertexAttributes);
    void unregisterVertexArray(uint32_t index);

    void setBoundVertexArrayIndex(uint32_t index);
    [[nodiscard]] uint32_t getBoundVertexArrayIndex() const;

    [[nodiscard]] GLVertexArray &getVertexArray(uint32_t index);
    [[nodiscard]] const GLVertexArray &getVertexArray(uint32_t index) const;

  private:
    [[nodiscard]] bool contains(const VertexAttributes &vertexAttributes) const;
    [[nodiscard]] uint32_t find(const VertexAttributes &vertexAttributes) const;
    void erase(uint32_t index);

  public:
    GLVertexArrayRegistry() = default;
    GLVertexArrayRegistry(const GLVertexArrayRegistry &) = delete;
    GLVertexArrayRegistry &operator=(const GLVertexArrayRegistry &) = delete;
    GLVertexArrayRegistry(GLVertexArrayRegistry &&) = delete;
    GLVertexArrayRegistry &operator=(GLVertexArrayRegistry &&) = delete;
    ~GLVertexArrayRegistry();

  private:
    using VertexArray = std::pair<GLVertexArray, uint32_t>;
    std::vector<VertexArray> m_vertexArrays;
    uint32_t m_boundVertexArrayIndex{0u};
};

} // namespace nox
