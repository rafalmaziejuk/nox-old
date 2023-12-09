#pragma once

#include "opengl/gl_state.h"

#include <nox/renderer.h>

namespace NOX {

class GLRenderer final : public Renderer {
  public:
    GLRenderer() = default;

    RendererBackend getRendererBackend() const override;

    std::unique_ptr<Swapchain> createSwapchain(const SwapchainDescriptor &descriptor) override;

    std::unique_ptr<Buffer> createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &vertexFormat) override;

    std::unique_ptr<Buffer> createIndexBuffer(const BufferDescriptor &descriptor, Format format) override;

    std::unique_ptr<Shader> createShader(const ShaderDescriptor &descriptor, std::string_view source) override;

    std::unique_ptr<GraphicsPipelineState> createGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor) override;

    std::unique_ptr<CommandList> createCommandList(const CommandListDescriptor &descriptor) override;

    std::unique_ptr<Texture> createTexture(const TextureDescriptor &descriptor) override;

    std::unique_ptr<RenderTarget> createRenderTarget(const RenderTargetDescriptor &descriptor) override;

  private:
    GLState m_state{};
};

} // namespace NOX
