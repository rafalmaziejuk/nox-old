#pragma once

#include <nox/framebuffer.h>

namespace nox {

class GLFramebuffer;

class FramebufferVisitor {
  public:
    virtual void visit(const GLFramebuffer &framebuffer) = 0;

  public:
    FramebufferVisitor() = default;
    FramebufferVisitor(const FramebufferVisitor &) = delete;
    FramebufferVisitor &operator=(const FramebufferVisitor &) = delete;
    virtual ~FramebufferVisitor() = default;
};

} // namespace nox
