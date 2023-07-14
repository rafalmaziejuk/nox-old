#pragma once

#include <nox/export.h>
#include <nox/vec.h>
#include <nox/format.h>
#include <nox/texture_types.h>

namespace NOX {

struct TextureDescriptor {
    Vector3D<uint32_t> size;
    TextureType type;
    Format format;
};

class NOX_EXPORT Texture {
  public:
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    virtual ~Texture();

  protected:
    Texture() = default;
};

} // namespace NOX
