#pragma once

#include "opengl/gl_program.h"
#include "opengl/gl_state.h"

#include <nox/graphics_pipeline_state.h>

namespace nox {

class GLGraphicsPipelineState final : public GraphicsPipelineState, public GLWithState {
  public:
    [[nodiscard]] static bool validateInput(const GraphicsPipelineStateDescription &description);

    GLGraphicsPipelineState(GraphicsPipelineStateDescription &description, GLState &state);
    ~GLGraphicsPipelineState() override;

    void bind() override;

    [[nodiscard]] bool bindShaderStages(const ShaderStages &shaderStages);

  private:
    std::unique_ptr<PipelineLayout> m_pipelineLayout{nullptr};
    GLProgram m_program{};
    uint32_t m_primitiveTopology{0u};
    uint32_t m_handle{0u};
};

} // namespace nox
