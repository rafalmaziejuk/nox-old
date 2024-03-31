#pragma once

#include "opengl/gl_vertex_array.h"

namespace nox {

class GLFramebuffer;
class GLRenderPass;
class GLGraphicsPipelineState;

struct GLState {
    const GLFramebuffer *currentFramebuffer{nullptr};
    const GLRenderPass *currentRenderPass{nullptr};
    const GLGraphicsPipelineState *currentPipeline{nullptr};

    uint32_t indexType;
    uint32_t primitiveTopology;
};

} // namespace nox
