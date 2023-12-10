#pragma once

#include <nox/export.h>
#include <nox/surface.h>

#include <variant>

namespace NOX {

struct OpenGLPresentMode {
    bool vSync;
};

using PresentMode = std::variant<OpenGLPresentMode>;

struct SwapchainDescriptor {
    SurfaceDescriptor surfaceDescriptor;
    PresentMode presentMode;
};

class NOX_EXPORT Swapchain {
  public:
    virtual void present() const = 0;

  public:
    Swapchain(const Swapchain &) = delete;
    Swapchain &operator=(const Swapchain &) = delete;
    Swapchain(Swapchain &&) = delete;
    Swapchain &operator=(Swapchain &&) = delete;
    virtual ~Swapchain() = default;

  protected:
    Swapchain() = default;
};

} // namespace NOX
