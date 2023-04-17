#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/renderer/common_types.h>

namespace NOX {

struct SwapChainDescriptor {
    PixelFormatDescriptor pixelFormatDescriptor;

    bool isVSync;
};

class NOX_EXPORT SwapChain : public NonCopyable {
  public:
    virtual void swap() const = 0;

    virtual void setVSync(bool value) = 0;
};

} // namespace NOX
