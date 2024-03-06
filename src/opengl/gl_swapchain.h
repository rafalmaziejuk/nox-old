#pragma once

#include "opengl/gl_program.h"
#include "opengl/gl_texture.h"
#include "opengl/gl_vertex_array.h"

#include <nox/swapchain.h>

namespace nox {

class GLContext;

class GLSwapchain final : public Swapchain {
  public:
    [[nodiscard]] static bool validateInput(const SwapchainDescriptor &descriptor);

    GLSwapchain(const SwapchainDescriptor &descriptor, std::unique_ptr<GLContext> context);

    Vector2D<uint32_t> getSize() const override;
    ImageFormat getSurfaceFormat() const override;
    std::vector<const Texture *> getSwapchainTextures() const override;

    void present() const override;

  private:
    GLProgram m_presentProgram{};
    GLVertexArray m_emptyVertexArray{};
    std::unique_ptr<GLContext> m_context{nullptr};
    GLTexture2D m_swapchainTexture;
    Vector2D<uint32_t> m_size;
};

} // namespace nox
