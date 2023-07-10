#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/renderer/pipeline_state_types.h>

#include <memory>

namespace NOX {

class RenderTarget;
class Shader;

struct PipelineStateDescriptor {
    std::unique_ptr<Shader> vertexShader;
    std::unique_ptr<Shader> fragmentShader;
    std::shared_ptr<RenderTarget> renderTarget;

    PrimitiveTopology primitiveTopology;
};

class NOX_EXPORT PipelineState : public NonCopyable {
  public:
    virtual ~PipelineState();
};

} // namespace NOX
