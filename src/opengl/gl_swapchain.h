#pragma once

#include "opengl/gl_render_target.h"

#include <nox/swapchain.h>

namespace nox {

class GLContext;

class GLSwapchain final : public Swapchain {
  public:
    [[nodiscard]] static bool validateInput(const SwapchainDescriptor &descriptor);

    GLSwapchain(const SwapchainDescriptor &descriptor);

    void present() const override;

    GLDefaultRenderTarget &getDefaultRenderTarget() { return m_renderTarget; }

  private:
    std::unique_ptr<GLContext> m_context{nullptr};
    GLDefaultRenderTarget m_renderTarget{};
};

} // namespace nox
