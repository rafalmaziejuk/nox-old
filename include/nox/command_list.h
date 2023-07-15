#pragma once

#include <nox/export.h>
#include <nox/vec.h>
#include <nox/command_list_types.h>

namespace NOX {

class Buffer;
class PipelineState;
class RenderPass;
class RenderTarget;

struct CommandListDescriptor {};

class NOX_EXPORT CommandList {
  public:
    CommandList(const CommandList &) = delete;
    CommandList &operator=(const CommandList &) = delete;
    virtual ~CommandList();

  protected:
    CommandList() = default;

  public:
    virtual void bindVertexBuffer(const Buffer &buffer) = 0;

    virtual void bindIndexBuffer(const Buffer &buffer) = 0;

    virtual void bindPipelineState(const PipelineState &pipeline) = 0;

    virtual void setViewport(const Viewport &viewport) = 0;

    virtual void setClearColor(const Vector4D<float> &color = {0.0f, 0.0f, 0.0f, 1.0f}) = 0;

    virtual void setClearDepth(float depth = 1.0f) = 0;

    virtual void setClearStencil(uint32_t stencil = 0u) = 0;

    virtual void clear(uint32_t mask) = 0;

    virtual void clearColor(const Vector4D<float> &color, uint8_t index = 0u) = 0;

    virtual void clearColor(const Vector4D<int32_t> &color, uint8_t index = 0u) = 0;

    virtual void clearColor(const Vector4D<uint32_t> &color, uint8_t index = 0u) = 0;

    virtual void clearDepth(float depth = 1.0f) = 0;

    virtual void clearStencil(uint32_t stencil = 0u) = 0;

    virtual void clearDepthStencil(float depth = 1.0f, uint32_t stencil = 0u) = 0;

    virtual void draw(uint32_t firstVertexIndex, uint32_t vertexCount) = 0;

    virtual void drawIndexed(uint32_t firstVertexIndex, uint32_t vertexCount) = 0;

    virtual void beginRenderPass(const RenderPass &renderPass) = 0;

    virtual void endRenderPass() = 0;
};

} // namespace NOX
