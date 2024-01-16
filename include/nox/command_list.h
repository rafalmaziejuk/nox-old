#pragma once

#include <nox/export.h>
#include <nox/framebuffer.h>
#include <nox/render_pass.h>
#include <nox/vector.h>
#include <nox/viewport.h>

#include <cstdint>
#include <variant>

namespace nox {

struct ClearDepthStencilValue {
    float depth;
    uint32_t stencil;
};

using ClearColorValue = std::variant<Vector4D<float>, Vector4D<int32_t>, Vector4D<uint32_t>>;
using ClearValue = std::variant<ClearColorValue, ClearDepthStencilValue>;
using ClearValues = std::vector<ClearValue>;

struct RenderPassBeginDescription {
    const RenderPass *renderPass;
    const Framebuffer *framebuffer;
    ClearValues clearValues;
};

struct CommandListDescription {};

class NOX_EXPORT CommandList {
  public:
    virtual void setViewport(const Viewport &viewport) = 0;

    virtual void beginRenderPass(const RenderPassBeginDescription &description) = 0;
    virtual void endRenderPass() = 0;

    virtual void draw(uint32_t firstVertexIndex, uint32_t vertexCount) = 0;
    virtual void drawIndexed(uint32_t firstVertexIndex, uint32_t vertexCount) = 0;

  public:
    CommandList(const CommandList &) = delete;
    CommandList &operator=(const CommandList &) = delete;
    CommandList(CommandList &&) = delete;
    CommandList &operator=(CommandList &&) = delete;
    virtual ~CommandList() = default;

  protected:
    CommandList() = default;
};

} // namespace nox
