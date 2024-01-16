#pragma once

#include "opengl/gl_state.h"

#include <nox/renderer.h>

namespace nox {

class GLRenderer final : public Renderer {
  public:
    GLRenderer() = default;

    RendererBackend getRendererBackend() const override;

    std::unique_ptr<Swapchain> createSwapchain(const SwapchainDescription &description) override;

    std::unique_ptr<Buffer> createVertexBuffer(const VertexBufferDescription &description) override;

    std::unique_ptr<Buffer> createIndexBuffer(const IndexBufferDescription &description) override;

    std::unique_ptr<Shader> createShader(const ShaderDescription &description, std::string_view source) override;

    std::unique_ptr<PipelineLayout> createPipelineLayout(PipelineLayoutDescription &description) override;

    std::unique_ptr<GraphicsPipelineState> createGraphicsPipelineState(GraphicsPipelineStateDescription &description) override;

    std::unique_ptr<CommandList> createCommandList(const CommandListDescription &description) override;

    std::shared_ptr<Texture> createTexture2D(const Texture2DDescription &description) override;

    std::unique_ptr<RenderPass> createRenderPass(const RenderPassDescription &description) override;

    std::unique_ptr<Framebuffer> createFramebuffer(const FramebufferDescription &description) override;

  private:
    GLState m_state{};
};

} // namespace nox
