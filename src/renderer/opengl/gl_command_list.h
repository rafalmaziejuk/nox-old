#pragma once

#include <nox/renderer/command_list.h>

#include <memory>

namespace NOX {

struct GLState;

class GLCommandList final : public CommandList {
  public:
    GLCommandList(const CommandListDescriptor &descriptor, std::shared_ptr<GLState> state);

    void bindVertexBuffer(const Buffer &buffer) override;
    void bindIndexBuffer(const Buffer &buffer) override;

    void bindPipelineState(const PipelineState &pipeline) override;

    void setViewport(const Viewport &viewport) override;
    void setClearColor(const Vector4D<float> &color) override;
    void setClearDepth(float depth) override;
    void setClearStencil(uint32_t stencil) override;
    void clear(uint8_t flags) override;

    void clearColor(const Vector4D<float> &color, uint8_t index) override;
    void clearColor(const Vector4D<int32_t> &color, uint8_t index) override;
    void clearColor(const Vector4D<uint32_t> &color, uint8_t index) override;
    void clearDepth(float depth) override;
    void clearStencil(uint32_t stencil) override;
    void clearDepthStencil(float depth, uint32_t stencil) override;

    void draw(uint32_t firstVertexIndex, uint32_t vertexCount) override;
    void drawIndexed(uint32_t firstVertexIndex, uint32_t vertexCount) override;

    void beginRenderPass(const RenderPass &renderPass) override;
    void endRenderPass() override;

  private:
    std::shared_ptr<GLState> m_state{nullptr};
};

} // namespace NOX
