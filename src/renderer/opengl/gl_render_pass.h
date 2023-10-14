#pragma once

#include <nox/render_pass.h>

namespace NOX {

class GLPipelineState;

class GLRenderPass final : public RenderPass {
  public:
    explicit GLRenderPass(const RenderPassDescriptor &descriptor);

    const GLPipelineState &getPipelineState() const;

    void setInput(uint32_t binding, const Resource &resource) override;

  private:
    std::shared_ptr<GLPipelineState> m_pipelineState{nullptr};
};

} // namespace NOX
