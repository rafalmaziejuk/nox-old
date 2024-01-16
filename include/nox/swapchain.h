#pragma once

#include <nox/export.h>
#include <nox/format.h>
#include <nox/framebuffer.h>
#include <nox/surface.h>
#include <nox/texture.h>

#include <variant>

namespace nox {

struct OpenGLPresentMode {
    bool vSync;
};

using PresentMode = std::variant<OpenGLPresentMode>;

struct SwapchainDescription {
    SurfaceDescription surfaceDescription;
    Vector2D<uint32_t> size;
    PresentMode presentMode;
};

class NOX_EXPORT Swapchain {
  public:
    [[nodiscard]] virtual Vector2D<uint32_t> getSize() const = 0;
    [[nodiscard]] virtual ImageFormat getSurfaceFormat() const = 0;
    [[nodiscard]] virtual std::vector<const Texture *> getPresentableTextures() const = 0;

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

} // namespace nox
