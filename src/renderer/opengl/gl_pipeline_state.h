#pragma once

#include <nox/pipeline_state.h>

namespace NOX {

class GLRenderTargetBase;

class GLPipelineState final : public PipelineState {
  public:
    explicit GLPipelineState(const PipelineStateDescriptor &descriptor);
    ~GLPipelineState() override;

    void bind() override;

  private:
    uint32_t m_handle{0u};
};

} // namespace NOX
