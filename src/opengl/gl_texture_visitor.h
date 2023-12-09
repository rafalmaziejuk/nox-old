#pragma once

#include "texture_visitor.h"
#include "opengl/gl_texture.h"

namespace NOX {

class GLTextureVisitor final : public TextureVisitor {
  public:
    void visit(const GLTexture &texture) override {
        m_handle = texture.getHandle();
    }

    uint32_t getHandle() const { return m_handle; }

  private:
    uint32_t m_handle{0u};
};

} // namespace NOX
