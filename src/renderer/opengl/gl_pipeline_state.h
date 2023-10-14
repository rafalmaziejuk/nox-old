#pragma once

#include <nox/pipeline_state.h>

namespace NOX {

class GLPipelineLayout;
class GLRenderTargetBase;

class GLPipelineState final : public PipelineState {
  public:
    explicit GLPipelineState(const PipelineStateDescriptor &descriptor);
    ~GLPipelineState() override;

    const GLPipelineLayout &getPipelineLayout() const;
    const GLRenderTargetBase &getRenderTarget() const;

    void bind() const;

  private:
    std::unique_ptr<GLPipelineLayout> m_pipelineLayout{nullptr};
    std::shared_ptr<GLRenderTargetBase> m_renderTarget{nullptr};
    uint32_t m_handle{0u};
};

} // namespace NOX
