#pragma once

#include "opengl/gl_context.h"

#include <nox/swapchain.h>

namespace NOX {

class GLDefaultRenderTarget;

class GLSwapchain final : public Swapchain, public GLWithContext {
  public:
    GLSwapchain(const SwapchainDescriptor &descriptor, GLContext &context);

    void swap() const override;

    void setVSync(bool value) override;

    const GLDefaultRenderTarget &getDefaultRenderTarget() const { return m_renderTarget; }

  private:
    GLDefaultRenderTarget m_renderTarget{};
};

} // namespace NOX
