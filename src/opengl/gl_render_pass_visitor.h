#pragma once

#include "render_pass_visitor.h"
#include "opengl/gl_render_pass.h"

namespace nox {

class GLRenderPassVisitor final : public RenderPassVisitor {
  public:
    void visit(const GLRenderPass &renderPass) override {
        m_renderPass = &renderPass;
    }

    const GLRenderPass &get() const { return *m_renderPass; }

  private:
    const GLRenderPass *m_renderPass{nullptr};
};

} // namespace nox
