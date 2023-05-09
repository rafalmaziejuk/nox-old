#pragma once

#include "renderer/opengl/gl_object.h"

namespace NOX {

class GLBuffer;
struct VertexFormat;

class GLVertexArray final : public GLObject {
  public:
    GLVertexArray();
    ~GLVertexArray();

    void setVertexBuffer(const GLBuffer &vertexBuffer, const VertexFormat &format);
    void setIndexBuffer(const GLBuffer &indexBuffer);

    void bind() const;

  private:
    using GLObject::getHandle;

    uint32_t m_currentBindingIndex{0u};
};

} // namespace NOX
