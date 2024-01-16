#pragma once

#include "opengl/gl_state.h"

#include <nox/command_list.h>

namespace nox {

class GLCommandList final : public CommandList, public GLWithState {
  public:
    [[nodiscard]] static bool validateInput(const RenderPassBeginDescription &description);

    GLCommandList(const CommandListDescription &description, GLState &state);

    void setViewport(const Viewport &viewport) override;

    void beginRenderPass(const RenderPassBeginDescription &description) override;
    void endRenderPass() override;

    void draw(uint32_t firstVertexIndex, uint32_t vertexCount) override;
    void drawIndexed(uint32_t firstVertexIndex, uint32_t vertexCount) override;
};

} // namespace nox
