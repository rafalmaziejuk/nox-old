#pragma once

#include <nox/texture.h>

namespace nox {

class GLTexture;
class GLDefaultFramebufferAttachment;

class TextureVisitor {
  public:
    virtual void visit(const GLTexture &texture) = 0;
    virtual void visit(const GLDefaultFramebufferAttachment &attachment) = 0;

  public:
    TextureVisitor() = default;
    TextureVisitor(const TextureVisitor &) = delete;
    TextureVisitor &operator=(const TextureVisitor &) = delete;
    virtual ~TextureVisitor() = default;
};

} // namespace nox
