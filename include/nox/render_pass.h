#pragma once

#include <nox/export.h>
#include <nox/pipeline_state.h>
#include <nox/resource.h>

#include <cstdint>
#include <memory>

namespace NOX {

struct RenderPassDescriptor {
    std::shared_ptr<PipelineState> pipelineState;
};

class NOX_EXPORT RenderPass {
  public:
    virtual void setInput(uint32_t binding, const Resource &resource) = 0;

  public:
    RenderPass(const RenderPass &) = delete;
    RenderPass &operator=(const RenderPass &) = delete;
    RenderPass(RenderPass &&) = delete;
    RenderPass &operator=(RenderPass &&) = delete;
    virtual ~RenderPass() = default;

  protected:
    RenderPass() = default;
};

} // namespace NOX
