#pragma once

#include <nox/texture.h>

namespace NOX {

class GLTexture;

class TextureVisitor {
  public:
    virtual void visit(const GLTexture &texture) = 0;

  public:
    TextureVisitor() = default;
    TextureVisitor(const TextureVisitor &) = delete;
    TextureVisitor &operator=(const TextureVisitor &) = delete;
    virtual ~TextureVisitor() = default;
};

} // namespace NOX
