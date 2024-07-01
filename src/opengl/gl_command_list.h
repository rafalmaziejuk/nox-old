#pragma once

#include <nox/command_list.h>

namespace nox {

class GLFramebuffer;
class GLRenderPass;

class GLCommandList final : public CommandList {
  public:
    explicit GLCommandList(const CommandListDescriptor &descriptor);

    void setViewport(const Viewport &viewport) override;

    void beginRenderPass(const RenderPassBeginDescriptor &descriptor) override;
    void endRenderPass() override;

    void bindGraphicsPipelineState(const GraphicsPipelineState &pipelineState) override;
    void bindVertexBuffer(const Buffer &buffer) override;
    void bindIndexBuffer(const Buffer &buffer) override;

    void draw(uint32_t firstVertexIndex, uint32_t vertexCount) override;
    void drawIndexed(uint32_t firstVertexIndex, uint32_t vertexCount) override;

  private:
    const GLFramebuffer *m_framebuffer{nullptr};
    const GLRenderPass *m_renderPass{nullptr};
    uint32_t m_indexType{0u};
    uint32_t m_primitiveTopology{0u};
};

} // namespace nox
