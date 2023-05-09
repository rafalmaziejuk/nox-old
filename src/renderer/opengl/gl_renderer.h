#pragma once

#include <nox/renderer/renderer.h>

namespace NOX {

class GLContext;

class GLRenderer final : public Renderer {
  public:
    explicit GLRenderer(const RendererDescriptor &descriptor);

    std::unique_ptr<SwapChain> createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) override;

  private:
    std::shared_ptr<GLContext> m_context{nullptr};
};

} // namespace NOX
