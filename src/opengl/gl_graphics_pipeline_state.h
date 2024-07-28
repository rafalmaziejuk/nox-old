#pragma once

#include "opengl/gl_program.h"

#include <nox/graphics_pipeline_state.h>

#include <memory>

namespace nox {

class GLGraphicsPipelineState final : public GraphicsPipelineState {
  public:
    [[nodiscard]] static std::unique_ptr<GLGraphicsPipelineState> create(const GraphicsPipelineStateDescriptor &descriptor);

    ~GLGraphicsPipelineState() override;

    [[nodiscard]] uint32_t getSubpassIndex() const {
        return m_subpassIndex;
    }

    [[nodiscard]] uint32_t getPrimitiveTopology() const {
        return m_primitiveTopology;
    }

    void bind() const;

  private:
    explicit GLGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor);

    [[nodiscard]] bool bindShaderStages(const ShaderStages &shaderStages) const;

  private:
    GLProgram m_program{};
    uint32_t m_subpassIndex{0u};
    uint32_t m_primitiveTopology{0u};
    uint32_t m_handle{0u};
};

} // namespace nox
