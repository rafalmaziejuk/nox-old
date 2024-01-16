#pragma once

#include <nox/export.h>

#include <cstdint>
#include <variant>

namespace nox {

struct WindowsSurfaceBackendDescription {
    void *windowHandle;
};

struct X11SurfaceBackendDescription {
    uint64_t windowHandle;
    void *displayHandle;
};

using SurfaceBackendDescription = std::variant<WindowsSurfaceBackendDescription,
                                              X11SurfaceBackendDescription>;

struct OpenGLSurfaceAttributesDescription {
    struct PixelFormatDescription {
        uint8_t colorBits{32u};
        uint8_t depthBits{24u};
        uint8_t stencilBits{8u};
    } pixelFormatDescription;
};

using SurfaceAttributesDescription = std::variant<OpenGLSurfaceAttributesDescription>;

struct SurfaceDescription {
    SurfaceBackendDescription surfaceBackendDescription;
    SurfaceAttributesDescription surfaceAttributesDescription;
};

class NOX_EXPORT Surface {
  public:
    Surface(const Surface &) = delete;
    Surface &operator=(const Surface &) = delete;
    Surface(Surface &&) = delete;
    Surface &operator=(Surface &&) = delete;
    virtual ~Surface() = default;

  protected:
    Surface() = default;
};

} // namespace nox
