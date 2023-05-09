#pragma once

#include <nox/renderer/renderer.h>

namespace NOX {

class GLBuffer;
class GLContext;
struct GLState;

class GLRenderer final : public Renderer {
  public:
    explicit GLRenderer(const RendererDescriptor &descriptor);

    std::unique_ptr<SwapChain> createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) override;

    std::unique_ptr<Buffer> createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &format) override;
    std::unique_ptr<Buffer> createIndexBuffer(const BufferDescriptor &descriptor, Format format) override;

    std::unique_ptr<Shader> createShaderFromString(const ShaderDescriptor &descriptor, std::string_view source) override;

    std::unique_ptr<PipelineState> createPipelineState(const PipelineStateDescriptor &descriptor) override;

  private:
    bool isVertexFormatUnique(const VertexFormat &format, uint32_t &index);

  private:
    std::shared_ptr<GLContext> m_context{nullptr};
    std::shared_ptr<GLState> m_state{nullptr};
};

} // namespace NOX
