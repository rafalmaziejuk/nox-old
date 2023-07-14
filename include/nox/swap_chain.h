#pragma once

#include <nox/export.h>
#include <nox/swap_chain_types.h>

#include <memory>

namespace NOX {

class RenderTarget;

struct SwapChainDescriptor {
    PixelFormatDescriptor pixelFormatDescriptor;

    bool isVSync;
};

class NOX_EXPORT SwapChain {
  public:
    SwapChain(const SwapChain &) = delete;
    SwapChain &operator=(const SwapChain &) = delete;
    virtual ~SwapChain();

  protected:
    SwapChain() = default;

  public:
    virtual void swap() const = 0;

    virtual void setVSync(bool value) = 0;

    virtual std::shared_ptr<RenderTarget> getRenderTarget() = 0;
};

} // namespace NOX
