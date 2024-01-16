#pragma once

#include "opengl/gl_texture.h"

#include <nox/swapchain.h>

namespace nox {

class GLContext;

class GLSwapchain final : public Swapchain {
  public:
    [[nodiscard]] static bool validateInput(const SwapchainDescription &description);

    GLSwapchain(const SwapchainDescription &description);

    Vector2D<uint32_t> getSize() const override;
    ImageFormat getSurfaceFormat() const override;
    std::vector<const Texture *> getPresentableTextures() const override;

    void present() const override;

  private:
    std::unique_ptr<GLContext> m_context{nullptr};
    std::unique_ptr<GLTexture2D> m_presentableTexture{nullptr};
    Vector2D<uint32_t> m_size;
};

} // namespace nox
