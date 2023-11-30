#pragma once

#include "opengl/linux/linux_gl_context.h"

using Display = struct _XDisplay;
using Window = uint64_t;

namespace NOX {

class X11GLContext final : public LinuxGLContext {
  public:
    explicit X11GLContext(const X11SurfaceBackendDescriptor &descriptor);
    ~X11GLContext() override;

    bool destroy() override;

  protected:
    bool setDisplayHandle() override;
    bool setSurfaceHandle(EGLConfig framebufferConfig) override;

  private:
    Display *m_handleDisplayX11{nullptr};
    Window m_handleWindowX11{0u};
};

} // namespace NOX
