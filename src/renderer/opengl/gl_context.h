#pragma once

#include <nox/non_copyable.h>
#include <nox/renderer/common_types.h>

#include <memory>

namespace NOX {

class Window;

class GLContext : public NonCopyable {
  public:
    GLContext(const OpenGLRendererConfig &config);
    ~GLContext();

    void createExtendedContext(const PixelFormatDescriptor &descriptor, const Window &window);

    void makeCurrent() const;

    void swapBuffers() const;
    void setSwapInterval(bool value);

  private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    OpenGLRendererConfig m_config;
};

} // namespace NOX
