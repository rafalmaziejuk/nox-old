#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/vector3d.h>
#include <nox/renderer/command_list_types.h>

#include <cstdint>

namespace NOX {

class Buffer;
class PipelineState;

struct CommandListDescriptor {};

class NOX_EXPORT CommandList : public NonCopyable {
  public:
    virtual ~CommandList();

    virtual void bindVertexBuffer(const Buffer &buffer) = 0;
    virtual void bindIndexBuffer(const Buffer &buffer) = 0;

    virtual void bindPipelineState(const PipelineState &pipeline) = 0;

    virtual void setViewport(const Viewport &viewport) = 0;
    virtual void setClearColor(const Vector3D<float> &color = {}) = 0;
    virtual void setClearDepth(float depth = 1.0f) = 0;
    virtual void setClearStencil(uint32_t stencil = 0u) = 0;

    virtual void clear(uint8_t flags) = 0;

    virtual void draw(uint32_t firstVertexIndex, uint32_t vertexCount) = 0;
    virtual void drawIndexed(uint32_t firstVertexIndex, uint32_t vertexCount) = 0;
};

} // namespace NOX
