#pragma once

#include <nox/export.h>

#include <variant>

namespace NOX {

struct WindowsSurfaceBackendDescriptor {
    void *windowHandle;
};

struct WaylandSurfaceBackendDescriptor {
    void *displayHandle;
    void *surfaceHandle;
};

struct X11SurfaceBackendDescriptor {
    void *windowHandle;
    void *displayHandle;
};

using SurfaceBackendDescriptor = std::variant<WindowsSurfaceBackendDescriptor,
                                              WaylandSurfaceBackendDescriptor,
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
    [[nodiscard]] virtual bool destroy() = 0;

  public:
    Surface(const Surface &) = delete;
    Surface &operator=(const Surface &) = delete;
    Surface(Surface &&) = delete;
    Surface &operator=(Surface &&) = delete;
    virtual ~Surface() = default;

  protected:
    Surface() = default;
};

} // namespace NOX
