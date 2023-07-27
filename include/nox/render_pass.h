#pragma once

#include <nox/export.h>
#include <nox/render_pass_types.h>

namespace NOX {

class PipelineState;
class RenderTarget;
class Resource;

struct RenderPassDescriptor {
    std::shared_ptr<PipelineState> pipelineState;
};

class NOX_EXPORT RenderPass {
  public:
    RenderPass(const RenderPass &) = delete;
    RenderPass &operator=(const RenderPass &) = delete;
    virtual ~RenderPass();

  protected:
    RenderPass() = default;

  public:
    virtual void setInput(uint32_t binding, const Resource &resource) = 0;
};

} // namespace NOX
