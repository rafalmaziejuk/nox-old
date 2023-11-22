#pragma once

#include "opengl/gl_render_target.h"

#include <nox/swapchain.h>

namespace NOX {

class GLContext;

class GLSwapchain final : public Swapchain {
  public:
    GLSwapchain(const SwapchainDescriptor &descriptor, std::shared_ptr<GLContext> context);

    void present() const override;

    GLDefaultRenderTarget &getDefaultRenderTarget() { return m_renderTarget; }

  private:
    std::shared_ptr<GLContext> m_context{nullptr};
    GLDefaultRenderTarget m_renderTarget{};
};

} // namespace NOX
