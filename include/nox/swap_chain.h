#pragma once

#include <nox/export.h>
#include <nox/render_target.h>

#include <cstdint>
#include <memory>

namespace NOX {

struct PixelFormatDescriptor {
    uint8_t colorBits{32u};
    uint8_t depthBits{24u};
    uint8_t stencilBits{8u};
};

struct SwapChainDescriptor {
    PixelFormatDescriptor pixelFormatDescriptor;

    bool isVSync;
};

class NOX_EXPORT SwapChain {
  public:
    virtual void swap() const = 0;

    virtual void setVSync(bool value) = 0;

    [[nodiscard]] virtual std::shared_ptr<RenderTarget> getRenderTarget() = 0;

  public:
    SwapChain(const SwapChain &) = delete;
    SwapChain &operator=(const SwapChain &) = delete;
    SwapChain(SwapChain &&) = delete;
    SwapChain &operator=(SwapChain &&) = delete;
    virtual ~SwapChain() = default;

  protected:
    SwapChain() = default;
};

} // namespace NOX
