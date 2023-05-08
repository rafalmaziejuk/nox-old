#pragma once

#include "renderer/opengl/gl_object.h"

#include <nox/renderer/pipeline_state.h>

namespace NOX {

class GLPipelineState final : public PipelineState, public GLObject {
  public:
    explicit GLPipelineState(const PipelineStateDescriptor &descriptor);
    ~GLPipelineState() override;

    void bind() const;
};

} // namespace NOX
