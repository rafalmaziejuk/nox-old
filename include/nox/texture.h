#pragma once

#include <nox/export.h>
#include <nox/format.h>
#include <nox/resource.h>
#include <nox/texture_types.h>
#include <nox/vec.h>

namespace NOX {

struct TextureDescriptor {
    Vector3D<uint32_t> size;
    TextureType type;
    Format format;
};

class NOX_EXPORT Texture : public Resource {
  public:
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    ~Texture() override;

  protected:
    Texture() = default;
};

} // namespace NOX
