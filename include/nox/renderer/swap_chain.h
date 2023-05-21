#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/renderer/swap_chain_types.h>

#include <memory>

namespace NOX {

class RenderTarget;

struct SwapChainDescriptor {
    PixelFormatDescriptor pixelFormatDescriptor;

    bool isVSync;
};

class NOX_EXPORT SwapChain {
  public:
    virtual ~SwapChain();

    virtual std::shared_ptr<RenderTarget> getRenderTarget() = 0;

    virtual void swap() const = 0;

    virtual void setVSync(bool value) = 0;
};

} // namespace NOX
