#pragma once

#include "opengl/gl_pipeline_layout.h"
#include "opengl/gl_program.h"
#include "opengl/gl_state.h"

#include <nox/graphics_pipeline_state.h>

namespace nox {

class GLGraphicsPipelineState final : public GraphicsPipelineState, public GLWithState {
  public:
    [[nodiscard]] static bool validateInput(const GraphicsPipelineStateDescriptor &descriptor);

    GLGraphicsPipelineState(GraphicsPipelineStateDescriptor &descriptor, GLState &state);
    ~GLGraphicsPipelineState() override;

    void bind() override;

    [[nodiscard]] bool bindShaderStages(const ShaderStages &shaderStages);

  private:
    GLPipelineLayout m_pipelineLayout;
    GLProgram m_program{};
    uint32_t m_subpassIndex{0u};
    uint32_t m_primitiveTopology{0u};
    uint32_t m_handle{0u};
};

} // namespace nox
