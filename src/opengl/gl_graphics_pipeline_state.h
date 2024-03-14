#pragma once

#include "opengl/gl_pipeline_layout.h"
#include "opengl/gl_program.h"

#include <nox/graphics_pipeline_state.h>

namespace nox {

class GLGraphicsPipelineState final : public GraphicsPipelineState {
  public:
    [[nodiscard]] static bool validateInput(const GraphicsPipelineStateDescriptor &descriptor);

    explicit GLGraphicsPipelineState(GraphicsPipelineStateDescriptor &descriptor);
    ~GLGraphicsPipelineState() override;

    [[nodiscard]] const GLPipelineLayout &getPipelineLayout() const {
        return m_pipelineLayout;
    }

    [[nodiscard]] uint32_t getSubpassIndex() const {
        return m_subpassIndex;
    }

    [[nodiscard]] uint32_t getPrimitiveTopology() const {
        return m_primitiveTopology;
    }

    void bind() const;

    [[nodiscard]] bool bindShaderStages(const ShaderStages &shaderStages) const;

  private:
    GLPipelineLayout m_pipelineLayout;
    GLProgram m_program{};
    uint32_t m_subpassIndex{0u};
    uint32_t m_primitiveTopology{0u};
    uint32_t m_handle{0u};
};

} // namespace nox
