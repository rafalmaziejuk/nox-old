#pragma once

#include <nox/format.h>

#include <memory>

namespace nox {

class GLVertexArray {
  public:
    explicit GLVertexArray(const VertexAttributes &vertexAttributes);
    ~GLVertexArray();

    void setVertexBuffer(uint32_t vertexBufferHandle);
    void setIndexBuffer(uint32_t indexBufferHandle) const;

    void bind() const;

  public:
    GLVertexArray(const GLVertexArray &) = delete;
    GLVertexArray &operator=(const GLVertexArray &) = delete;
    GLVertexArray(GLVertexArray &&other) = delete;
    GLVertexArray &operator=(GLVertexArray &&other) = delete;

  private:
    uint32_t m_currentBindingIndex{0u};
    uint32_t m_stride{0u};
    uint32_t m_handle{0u};
};

class GLVertexArrayRegistry {
  public:
    void registerVertexArray(const VertexAttributes &vertexAttributes);

    [[nodiscard]] GLVertexArray &operator[](uint32_t index);
    [[nodiscard]] const GLVertexArray &operator[](uint32_t index) const;

    [[nodiscard]] bool contains(const VertexAttributes &vertexAttributes) const;
    [[nodiscard]] uint32_t find(const VertexAttributes &vertexAttributes) const;

  public:
    GLVertexArrayRegistry() = default;
    GLVertexArrayRegistry(const GLVertexArrayRegistry &) = delete;
    GLVertexArrayRegistry &operator=(const GLVertexArrayRegistry &) = delete;
    GLVertexArrayRegistry(GLVertexArrayRegistry &&) = delete;
    GLVertexArrayRegistry &operator=(GLVertexArrayRegistry &&) = delete;

  private:
    using VertexArray = std::unique_ptr<GLVertexArray>;
    using VertexArrayEntry = std::pair<VertexAttributes, VertexArray>;
    using VertexArraysEntries = std::vector<VertexArrayEntry>;

    VertexArraysEntries m_vertexArraysEntries;
};

} // namespace nox
