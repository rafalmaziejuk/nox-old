#pragma once

#include <nox/export.h>

#include <cstdint>
#include <variant>

namespace nox {

struct WindowsSurfaceBackendDescriptor {
    void *windowHandle;
};

struct X11SurfaceBackendDescriptor {
    uint64_t windowHandle;
    void *displayHandle;
};

using SurfaceBackendDescriptor = std::variant<WindowsSurfaceBackendDescriptor,
                                              X11SurfaceBackendDescriptor>;

struct OpenGLSurfaceAttributesDescriptor {
    struct PixelFormatDescriptor {
        uint8_t colorBits{32u};
        uint8_t depthBits{24u};
        uint8_t stencilBits{8u};
    } pixelFormatDescriptor;
};

using SurfaceAttributesDescriptor = std::variant<OpenGLSurfaceAttributesDescriptor>;

struct SurfaceDescriptor {
    SurfaceBackendDescriptor surfaceBackendDescriptor;
    SurfaceAttributesDescriptor surfaceAttributesDescriptor;
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
