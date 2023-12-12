#pragma once

#include <nox/export.h>
#include <nox/format.h>
#include <nox/vector.h>

#include <cstdint>

namespace nox {

class TextureVisitor;

enum class TextureType {
    TEXTURE2D
};

struct TextureDescriptor {
    ImageFormat format;
};

struct Texture2DDescriptor : TextureDescriptor {
    Vector2D<uint32_t> size;
};

class NOX_EXPORT Texture {
  public:
    virtual TextureType getTextureType() const = 0;

    virtual void accept(TextureVisitor &visitor) const = 0;

  public:
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    Texture(Texture &&) = delete;
    Texture &operator=(Texture &&) = delete;
    virtual ~Texture() = default;

  protected:
    Texture() = default;
};

} // namespace nox
