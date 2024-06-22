#pragma once

#include <nox/swapchain.h>

namespace nox {

class GLContext;
class GLProgram;
class GLTexture2D;
class GLVertexArray;
class GLVertexArrayRegistry;

class GLSwapchain final : public Swapchain {
  public:
    [[nodiscard]] static std::unique_ptr<GLSwapchain> create(const SwapchainDescriptor &descriptor,
                                                             std::unique_ptr<GLContext> context,
                                                             std::shared_ptr<GLVertexArrayRegistry> registry);

    GLSwapchain(std::unique_ptr<GLContext> context,
                std::unique_ptr<GLProgram> program,
                std::unique_ptr<GLTexture2D> texture,
                std::shared_ptr<GLVertexArrayRegistry> registry,
                Vector2D<uint32_t> size);
    ~GLSwapchain() override;

    Vector2D<uint32_t> getSize() const override;
    ImageFormat getSurfaceFormat() const override;
    std::vector<const Texture *> getSwapchainTextures() const override;

    void present() const override;

  private:
    std::unique_ptr<GLContext> m_context{nullptr};
    std::unique_ptr<GLProgram> m_presentationProgram{nullptr};
    std::unique_ptr<GLTexture2D> m_presentationTexture{nullptr};
    std::shared_ptr<GLVertexArrayRegistry> m_vertexArrayRegistry{nullptr};
    std::shared_ptr<GLVertexArray> m_vertexArray{nullptr};
    Vector2D<uint32_t> m_size{};
};

} // namespace nox
