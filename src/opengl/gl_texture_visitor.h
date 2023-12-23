#pragma once

#include "texture_visitor.h"
#include "opengl/gl_texture.h"

namespace nox {

class GLTextureVisitor final : public TextureVisitor {
  public:
    void visit(const GLTexture &texture) override {
        m_texture = &texture;
    }

    void visit([[maybe_unused]] const GLDefaultFramebufferAttachment &attachment) override {
        m_isDefaultFramebufferAttachment = true;
    }

    [[nodiscard]] const GLTexture &get() const { return *m_texture; }
    [[nodiscard]] bool isDefaultFramebufferAttachment() const { return m_isDefaultFramebufferAttachment; }

  private:
    const GLTexture *m_texture{nullptr};
    bool m_isDefaultFramebufferAttachment{false};
};

} // namespace nox
