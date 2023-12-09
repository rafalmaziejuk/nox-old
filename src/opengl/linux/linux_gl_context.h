#pragma once

#include "opengl/gl_context.h"

using EGLSurface = void *;
using EGLDisplay = void *;
using EGLContext = void *;
using EGLConfig = void *;

namespace NOX {

class LinuxGLContext : public GLContext {
  public:
    ~LinuxGLContext() override;

    [[nodiscard]] bool initialize(const OpenGLSurfaceAttributesDescriptor &descriptor);

    void swapBuffers() const override;
    void setSwapInterval(bool value) const override;

  protected:
    [[nodiscard]] virtual bool setDisplayHandle() = 0;
    [[nodiscard]] virtual bool setSurfaceHandle(EGLConfig framebufferConfig) = 0;

  protected:
    EGLDisplay m_handleDisplay{nullptr};
    EGLSurface m_handleSurface{nullptr};
    EGLContext m_handleRenderingContext{nullptr};
};

} // namespace NOX
