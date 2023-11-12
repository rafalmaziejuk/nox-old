#pragma once

#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_state.h"

#include <nox/renderer.h>

namespace NOX {

class GLBuffer;
class GLCommandList;
class GLSwapChain;

class GLRenderer final : public Renderer {
  public:
    GLRenderer() = default;

    RendererBackend getRendererBackend() const override;

    ShaderRegistry &getShaderRegistry() override;

    const ShaderRegistry &getShaderRegistry() const override;

    std::unique_ptr<SwapChain> createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) override;

    std::unique_ptr<Buffer> createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &vertexFormat) override;

    std::unique_ptr<Buffer> createIndexBuffer(const BufferDescriptor &descriptor, Format format) override;

    std::unique_ptr<GraphicsPipelineState> createGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor) override;

    std::unique_ptr<CommandList> createCommandList(const CommandListDescriptor &descriptor) override;

    std::unique_ptr<Texture> createTexture(const TextureDescriptor &descriptor) override;

    std::unique_ptr<RenderTarget> createRenderTarget(const RenderTargetDescriptor &descriptor) override;

  private:
    GLContext m_context{};
    GLState m_state{};
};

} // namespace NOX
