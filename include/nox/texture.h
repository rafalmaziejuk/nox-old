#pragma once

#include <nox/common.h>
#include <nox/export.h>
#include <nox/vector.h>

#include <cstdint>

namespace NOX {

enum class TextureType : uint8_t {
    NONE,
    TEXTURE2D
};

struct TextureDescriptor {
    Vector3D<uint32_t> size;
    TextureType type;
    Format format;
};

class NOX_EXPORT Texture {
  public:
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    Texture(Texture &&) = delete;
    Texture &operator=(Texture &&) = delete;
    virtual ~Texture() = default;

  protected:
    Texture() = default;
};

} // namespace NOX
