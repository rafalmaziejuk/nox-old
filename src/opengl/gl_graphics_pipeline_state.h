#pragma once

#include "opengl/gl_program.h"
#include "opengl/gl_state.h"

#include <nox/graphics_pipeline_state.h>

namespace NOX {

class GLGraphicsPipelineState final : public GraphicsPipelineState, public GLWithState {
  public:
    [[nodiscard]] static bool validateInput(const GraphicsPipelineStateDescriptor &descriptor);

    GLGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor, GLState &state);
    ~GLGraphicsPipelineState() override;

    void bind() override;

    bool bindShaderStages(const ShaderStages &shaderStages);

  private:
    GLProgram m_program{};
    uint32_t m_primitiveTopology{0u};
    uint32_t m_handle{0u};
};

} // namespace NOX
