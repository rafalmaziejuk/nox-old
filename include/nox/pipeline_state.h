#pragma once

#include <nox/export.h>
#include <nox/pipeline_state_types.h>

#include <memory>

namespace NOX {

class RenderTarget;
class Shader;

struct PipelineStateDescriptor {
    PipelineLayoutDescriptor pipelineLayoutDescriptor;
    std::unique_ptr<Shader> vertexShader;
    std::unique_ptr<Shader> fragmentShader;
    std::shared_ptr<RenderTarget> renderTarget;

    PrimitiveTopology primitiveTopology;
};

class NOX_EXPORT PipelineState {
  public:
    PipelineState(const PipelineState &) = delete;
    PipelineState &operator=(const PipelineState &) = delete;
    virtual ~PipelineState();

  protected:
    PipelineState() = default;
};

} // namespace NOX
