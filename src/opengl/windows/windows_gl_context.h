#include "opengl/gl_context.h"

#include <windows.h>

namespace nox {

class WindowsGLContext final : public GLContext {
  public:
    ~WindowsGLContext() override;

    [[nodiscard]] bool initialize(const SurfaceDescriptor &descriptor);

    void makeCurrent() const override;
    void swapBuffers() const override;
    void setSwapInterval(bool interval) const override;

  private:
    bool preloadBackend() override;

  private:
    HWND m_handleWindow{nullptr};
    HDC m_handleDeviceContext{nullptr};
    HGLRC m_handleRenderingContext{nullptr};
};

} // namespace nox
