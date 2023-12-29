#pragma once

#include <nox/render_pass.h>

namespace nox {

class GLRenderPass;

class RenderPassVisitor {
  public:
    virtual void visit(const GLRenderPass &renderPass) = 0;

  public:
    RenderPassVisitor() = default;
    RenderPassVisitor(const RenderPassVisitor &) = delete;
    RenderPassVisitor &operator=(const RenderPassVisitor &) = delete;
    virtual ~RenderPassVisitor() = default;
};

} // namespace nox
