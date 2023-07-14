#pragma once

#include <nox/pipeline_state.h>

namespace NOX {

class GLRenderTargetBase;

class GLPipelineState final : public PipelineState {
  public:
    GLPipelineState(const GLPipelineState &) = delete;
    GLPipelineState &operator=(const GLPipelineState &) = delete;

  public:
    explicit GLPipelineState(const PipelineStateDescriptor &descriptor);
    ~GLPipelineState() override;

    const GLRenderTargetBase *getRenderTarget() const;

    void bind() const;

  private:
    std::shared_ptr<RenderTarget> m_renderTarget{nullptr};
    uint32_t m_handle{0u};
};

} // namespace NOX
