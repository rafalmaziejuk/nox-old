#pragma once

#include "opengl/gl_vertex_array.h"

namespace nox {

class GLFramebuffer;
class GLRenderPass;

struct GLState {
    GLVertexArrayRegistry vertexArrayRegistry;
    const GLFramebuffer *currentFramebuffer{nullptr};
    const GLRenderPass *currentRenderPass{nullptr};

    uint32_t indexType;
    uint32_t primitiveTopology;
};

struct GLWithState {
    explicit GLWithState(GLState &state) : state{&state} {}
    virtual ~GLWithState() = default;

    GLWithState(const GLWithState &) = delete;
    GLWithState &operator=(const GLWithState &) = delete;

    GLState *state{nullptr};
};

} // namespace nox
