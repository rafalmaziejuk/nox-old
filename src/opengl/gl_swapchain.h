#pragma once

#include <nox/swapchain.h>

namespace nox {

class GLContext;

class GLSwapchain final : public Swapchain {
  public:
    [[nodiscard]] static std::unique_ptr<GLSwapchain> create(const SwapchainDescriptor &descriptor,
                                                             std::unique_ptr<GLContext> context);

    GLSwapchain(const SwapchainDescriptor &descriptor,
                std::unique_ptr<GLContext> context);

    Vector2D<uint32_t> getSize() const override;
    ImageFormat getSurfaceFormat() const override;
    std::vector<const Texture *> getSwapchainTextures() const override;

    void present() const override;

  private:
    std::unique_ptr<GLContext> m_context{nullptr};
    Vector2D<uint32_t> m_size{};
};

} // namespace nox
