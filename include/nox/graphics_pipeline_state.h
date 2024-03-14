#pragma once

#include <nox/export.h>
#include <nox/pipeline_layout.h>
#include <nox/render_pass.h>
#include <nox/shader.h>

#include <cstdint>
#include <vector>

namespace nox {

using ShaderStages = std::vector<const Shader *>;

enum class PrimitiveTopology {
    TRIANGLE_LIST
};

struct GraphicsPipelineStateDescriptor {
    ShaderStages shaderStages;
    PipelineLayoutDescriptor pipelineLayoutDescriptor;
    const RenderPass *renderPass;
    uint32_t subpassIndex;
    PrimitiveTopology primitiveTopology;
};

class NOX_EXPORT GraphicsPipelineState {
  public:
    GraphicsPipelineState(const GraphicsPipelineState &) = delete;
    GraphicsPipelineState &operator=(const GraphicsPipelineState &) = delete;
    GraphicsPipelineState(GraphicsPipelineState &&) = delete;
    GraphicsPipelineState &operator=(GraphicsPipelineState &&) = delete;
    virtual ~GraphicsPipelineState() = default;

  protected:
    GraphicsPipelineState() = default;
};

} // namespace nox
