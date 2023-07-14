#pragma once

#include <nox/renderer.h>

namespace NOX {

class GLBuffer;
class GLCommandList;
class GLContext;
struct GLState;
class GLSwapChain;

class GLRenderer final : public Renderer {
  public:
    explicit GLRenderer(const RendererDescriptor &descriptor);

    std::unique_ptr<SwapChain> createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) override;

    std::unique_ptr<Buffer> createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &format) override;

    std::unique_ptr<Buffer> createIndexBuffer(const BufferDescriptor &descriptor, Format format) override;

    std::unique_ptr<Shader> createShaderFromString(const ShaderDescriptor &descriptor, std::string_view source) override;

    std::unique_ptr<PipelineState> createPipelineState(const PipelineStateDescriptor &descriptor) override;

    std::unique_ptr<CommandList> createCommandList(const CommandListDescriptor &descriptor) override;

    std::unique_ptr<Texture> createTexture(const TextureDescriptor &descriptor) override;

    std::unique_ptr<RenderTarget> createRenderTarget(const RenderTargetDescriptor &descriptor) override;

    std::unique_ptr<RenderPass> createRenderPass(const RenderPassDescriptor &descriptor) override;

  private:
    bool isVertexFormatUnique(const VertexFormat &format, uint32_t &index);

  private:
    std::shared_ptr<GLContext> m_context{nullptr};
    std::shared_ptr<GLState> m_state{nullptr};
};

} // namespace NOX
