#pragma once

#include "framebuffer_visitor.h"
#include "opengl/gl_framebuffer.h"

namespace nox {

class GLFramebufferVisitor final : public FramebufferVisitor {
  public:
    void visit(const GLFramebuffer &framebuffer) override {
        m_framebuffer = &framebuffer;
    }

    const GLFramebuffer &get() const { return *m_framebuffer; }

  private:
    const GLFramebuffer *m_framebuffer{nullptr};
};

} // namespace nox
