#pragma once

#include "renderer/opengl/gl_context.h"

#include <nox/swap_chain.h>

namespace NOX {

class GLDefaultRenderTarget;

class GLSwapChain final : public SwapChain, public GLWithContext {
  public:
    GLSwapChain(const SwapChainDescriptor &descriptor, GLContext &context);

    void swap() const override;

    void setVSync(bool value) override;

    std::shared_ptr<RenderTarget> getRenderTarget() override;

  private:
    std::shared_ptr<GLDefaultRenderTarget> m_renderTarget{nullptr};
};

} // namespace NOX
