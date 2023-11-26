#pragma once

#include <nox/common.h>

#include <memory>

namespace NOX {

class GLVertexArray {
  public:
    explicit GLVertexArray(const VertexFormat &vertexFormat);
    ~GLVertexArray();

    void setVertexBuffer(uint32_t vertexBufferHandle);
    void setIndexBuffer(uint32_t indexBufferHandle) const;

    void bind() const;

  public:
    GLVertexArray(const GLVertexArray &) = delete;
    GLVertexArray &operator=(const GLVertexArray &) = delete;

  private:
    uint32_t m_currentBindingIndex{0u};
    uint32_t m_handle{0u};
    uint32_t m_stride{0u};
};

class GLVertexArrayRegistry {
  public:
    [[nodiscard]] uint32_t registerVertexArray(const VertexFormat &vertexFormat);

    [[nodiscard]] GLVertexArray &operator[](uint32_t index);
    [[nodiscard]] const GLVertexArray &operator[](uint32_t index) const;

    void setBoundVertexArrayIndex(uint32_t index);
    [[nodiscard]] uint32_t getBoundVertexArrayIndex() const;

  private:
    [[nodiscard]] uint32_t find(const VertexFormat &vertexFormat) const;

  public:
    GLVertexArrayRegistry() = default;
    GLVertexArrayRegistry(const GLVertexArrayRegistry &) = delete;
    GLVertexArrayRegistry &operator=(const GLVertexArrayRegistry &) = delete;
    GLVertexArrayRegistry(GLVertexArrayRegistry &&) = delete;
    GLVertexArrayRegistry &operator=(GLVertexArrayRegistry &&) = delete;

  private:
    static constexpr uint32_t invalidVertexArrayIndex = ~0u;

    std::vector<std::unique_ptr<GLVertexArray>> m_vertexArrays;
    std::vector<VertexFormat> m_vertexFormats;
    uint32_t m_boundVertexArrayIndex{invalidVertexArrayIndex};
};

} // namespace NOX
