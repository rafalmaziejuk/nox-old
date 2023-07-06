#pragma once

#include <nox/renderer/pipeline_state.h>

namespace NOX {

class GLPipelineState final : public PipelineState {
  public:
    explicit GLPipelineState(const PipelineStateDescriptor &descriptor);
    ~GLPipelineState() override;

    void bind() const;

  private:
    uint32_t m_handle{0u};
};

} // namespace NOX
