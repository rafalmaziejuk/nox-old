#pragma once

#include "texture_visitor.h"
#include "opengl/gl_texture.h"

namespace nox {

class GLTextureVisitor final : public TextureVisitor {
  public:
    void visit(const GLTexture &texture) override {
        m_texture = &texture;
    }

    const GLTexture &get() const { return *m_texture; }

  private:
    const GLTexture *m_texture{nullptr};
};

} // namespace nox
