#pragma once

#include <nox/renderer/renderer.h>

namespace NOX {

class GLContext;
class GLSwapChain;

class GLRenderer final : public Renderer {
  public:
    GLRenderer(const RendererDescriptor &descriptor);

    std::shared_ptr<SwapChain> createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) override;

  private:
    std::shared_ptr<GLContext> m_context{nullptr};
    std::shared_ptr<GLSwapChain> m_swapChain{nullptr};

    RendererDescriptor m_descriptor;
};

} // namespace NOX
