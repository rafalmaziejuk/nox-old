#include "opengl/gl_context.h"

#include <windows.h>

namespace NOX {

class WindowsGLContext final : public GLContext {
  public:
    explicit WindowsGLContext(const WindowsSurfaceBackendDescriptor &descriptor);
    ~WindowsGLContext() override;

    [[nodiscard]] bool initialize(const OpenGLSurfaceAttributesDescriptor &descriptor);

    bool destroy() override;

    void swapBuffers() const override;
    void setSwapInterval(bool value) const override;

  private:
    HWND m_handleWindow{nullptr};
    HDC m_handleDeviceContext{nullptr};
    HGLRC m_handleRenderingContext{nullptr};
};

} // namespace NOX
