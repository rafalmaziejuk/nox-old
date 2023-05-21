#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/renderer/render_pass_types.h>

namespace NOX {

class RenderTarget;
class PipelineState;

struct RenderPassDescriptor {
    std::shared_ptr<PipelineState> pipelineState;
};

class NOX_EXPORT RenderPass : public NonCopyable {
  public:
    virtual ~RenderPass();

    // virtual const PipelineState &getPipelineState() const = 0;

    // virtual const RenderTarget &getRenderTarget() const = 0;
};

} // namespace NOX
