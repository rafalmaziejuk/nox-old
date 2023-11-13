#pragma once

#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_vertex_array.h"

namespace NOX {

struct GLState {
    GLVertexArrayRegistry vertexArrayRegistry;

    const GLRenderTargetBase *currentRenderTarget;
    uint32_t indexType;
    uint32_t primitiveTopology;
};

class GLWithState {
  public:
    explicit GLWithState(GLState &state) : m_state{&state} {}
    virtual ~GLWithState() = default;

    GLState &getState() const { return *m_state; }

  public:
    GLWithState(const GLWithState &) = delete;
    GLWithState &operator=(const GLWithState &) = delete;

  private:
    GLState *m_state{nullptr};
};

} // namespace NOX
