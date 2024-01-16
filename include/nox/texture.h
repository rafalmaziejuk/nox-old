#pragma once

#include <nox/export.h>
#include <nox/format.h>
#include <nox/vector.h>

#include <cstdint>

namespace nox {

struct TextureSubresourceDescription {
    uint32_t baseMipmapLevel;
};

struct TextureWriteDescription {
    void *data;
    ImageFormat dataFormat;
    TextureSubresourceDescription subresource;
    Vector3D<uint32_t> size;
    Vector3D<uint32_t> offset;
};

enum class TextureType {
    TEXTURE2D
};

struct TextureDescription {
    ImageFormat format;
};

struct Texture2DDescription : TextureDescription {
    Vector2D<uint32_t> size;
};

class NOX_EXPORT Texture {
  public:
    virtual void write(const TextureWriteDescription &description) = 0;

    [[nodiscard]] virtual TextureType getType() const = 0;
    [[nodiscard]] virtual ImageFormat getFormat() const = 0;

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
