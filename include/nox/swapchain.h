#pragma once

#include <nox/export.h>

#include <cstdint>
#include <memory>

namespace NOX {

struct PixelFormatDescriptor {
    uint8_t colorBits{32u};
    uint8_t depthBits{24u};
    uint8_t stencilBits{8u};
};

struct SwapchainDescriptor {
    PixelFormatDescriptor pixelFormatDescriptor;

    bool isVSync;
};

class NOX_EXPORT Swapchain {
  public:
    virtual void swap() const = 0;

    virtual void setVSync(bool value) = 0;

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
