#pragma once

#include <nox/export.h>
#include <nox/pipeline_layout.h>
#include <nox/shader.h>

#include <vector>

namespace nox {

using ShaderStages = std::vector<const Shader *>;

enum class PrimitiveTopology {
    TRIANGLE_LIST
};

struct GraphicsPipelineStateDescription {
    ShaderStages shaderStages;
    std::unique_ptr<PipelineLayout> pipelineLayout;
    PrimitiveTopology primitiveTopology;
};

class NOX_EXPORT GraphicsPipelineState {
  public:
    virtual void bind() = 0;

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
