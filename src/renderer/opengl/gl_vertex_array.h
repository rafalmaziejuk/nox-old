#pragma once

namespace NOX {

class GLBuffer;
struct VertexFormat;

class GLVertexArray {
  public:
    GLVertexArray();
    ~GLVertexArray();

    void setVertexBuffer(const GLBuffer &vertexBuffer, const VertexFormat &format);
    void setIndexBuffer(const GLBuffer &indexBuffer);

    void bind() const;

  private:
    uint32_t m_currentBindingIndex{0u};
    uint32_t m_handle{0u};
};

} // namespace NOX
