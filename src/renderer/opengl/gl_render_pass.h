#pragma once

#include <nox/renderer/render_pass.h>

namespace NOX {

class GLPipelineState;

class GLRenderPass final : public RenderPass {
  public:
    GLRenderPass(const RenderPassDescriptor &descriptor);

    bool validate() const;

    const GLPipelineState *getPipelineState() const;

  private:
    std::shared_ptr<PipelineState> m_pipelineState{nullptr};
};

} // namespace NOX
