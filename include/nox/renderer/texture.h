#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/vec.h>
#include <nox/renderer/format.h>
#include <nox/renderer/texture_types.h>

namespace NOX {

struct TextureDescriptor {
    Vector3D<uint32_t> size;
    TextureType type;
    Format format;
};

class NOX_EXPORT Texture {
  public:
    virtual ~Texture();
};

} // namespace NOX
