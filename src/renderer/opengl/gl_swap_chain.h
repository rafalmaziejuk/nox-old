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

    const GLDefaultRenderTarget &getDefaultRenderTarget() const { return m_renderTarget; }

  private:
    GLDefaultRenderTarget m_renderTarget{};
};

} // namespace NOX
