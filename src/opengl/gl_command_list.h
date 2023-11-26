#pragma once

#include "opengl/gl_state.h"

#include <nox/command_list.h>

namespace NOX {

class GLCommandList final : public CommandList, public GLWithState {
  public:
    GLCommandList(const CommandListDescriptor &descriptor, GLState &state);

    void setViewport(const Viewport &viewport) override;
    void setClearColor(const Vector4D<float> &color) override;
    void setClearDepth(float depth) override;
    void setClearStencil(uint32_t stencil) override;
    void clear(uint32_t mask) override;

    void clearColor(const Vector4D<float> &color, uint8_t index) override;
    void clearColor(const Vector4D<int32_t> &color, uint8_t index) override;
    void clearColor(const Vector4D<uint32_t> &color, uint8_t index) override;
    void clearDepth(float depth) override;
    void clearStencil(uint32_t stencil) override;
    void clearDepthStencil(float depth, uint32_t stencil) override;

    void draw(uint32_t firstVertexIndex, uint32_t vertexCount) override;
    void drawIndexed(uint32_t firstVertexIndex, uint32_t vertexCount) override;
};

} // namespace NOX
