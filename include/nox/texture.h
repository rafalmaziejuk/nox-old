#pragma once

#include <nox/export.h>
#include <nox/format.h>
#include <nox/vector.h>

#include <cstdint>

namespace nox {

struct TextureSubresourceDescriptor {
    uint32_t baseMipmapLevel;
};

struct TextureWriteDescriptor {
    void *data;
    ImageFormat dataFormat;
    TextureSubresourceDescriptor subresource;
    Vector3D<uint32_t> size;
    Vector3D<uint32_t> offset;
};

enum class TextureType {
    NONE,
    TEXTURE2D,
    MAX
};

struct TextureDescriptor {
    ImageFormat format;
};

struct Texture2DDescriptor : TextureDescriptor {
    Vector2D<uint32_t> size;
};

class NOX_EXPORT Texture {
  public:
    virtual void write(const TextureWriteDescriptor &descriptor) = 0;

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
