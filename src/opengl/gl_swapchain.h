#pragma once

#include "opengl/gl_program.h"
#include "opengl/gl_texture.h"

#include <nox/swapchain.h>

namespace nox {

class GLContext;

class GLSwapchain final : public Swapchain {
  public:
    [[nodiscard]] static std::unique_ptr<GLSwapchain> create(const SwapchainDescriptor &descriptor, std::unique_ptr<GLContext> context);

    GLSwapchain(std::unique_ptr<GLContext> context, Vector2D<uint32_t> size);

    Vector2D<uint32_t> getSize() const override;
    ImageFormat getSurfaceFormat() const override;
    std::vector<const Texture *> getSwapchainTextures() const override;

    void present() const override;

  private:
    [[nodiscard]] bool initializePresentationProgram();

  private:
    std::unique_ptr<GLContext> m_context{nullptr};
    GLProgram m_presentationProgram{};
    GLTexture2D m_presentationTexture;
    Vector2D<uint32_t> m_size;
};

} // namespace nox
