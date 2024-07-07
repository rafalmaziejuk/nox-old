#pragma once

#include <nox/export.h>
#include <nox/shader.h>

#include <cstdint>
#include <vector>

namespace nox {

using ShaderStages = std::vector<const Shader *>;

enum class PrimitiveTopology {
    NONE,
    TRIANGLE_LIST,
    MAX
};

struct GraphicsPipelineStateDescriptor {
    ShaderStages shaderStages;
    PrimitiveTopology primitiveTopology;
    uint32_t subpassIndex;
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
