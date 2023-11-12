#pragma once

#include <nox/export.h>
#include <nox/shader.h>

#include <cstdint>
#include <vector>

namespace NOX {

using ShaderStage = ShaderType;
using ShaderStages = std::vector<ShaderHandle>;

enum class PrimitiveTopology : uint8_t {
    TRIANGLE_LIST
};

struct GraphicsPipelineStateDescriptor {
    ShaderStages shaderStages;
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

} // namespace NOX
