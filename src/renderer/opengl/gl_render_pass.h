#pragma once

#include <nox/render_pass.h>

namespace NOX {

class GLPipelineState;

class GLRenderPass final : public RenderPass {
  public:
    explicit GLRenderPass(const RenderPassDescriptor &descriptor);

    void setInput(uint32_t index, const Texture &texture) override;

    const GLPipelineState *getPipelineState() const;

  private:
    std::shared_ptr<PipelineState> m_pipelineState{nullptr};
};

} // namespace NOX
