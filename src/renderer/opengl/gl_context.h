#pragma once

#include <nox/renderer/renderer_types.h>

#include <memory>

namespace NOX {

struct PixelFormatDescriptor;
class Window;

class GLContext {
  public:
    GLContext(const GLContext &) = delete;
    GLContext &operator=(const GLContext &) = delete;

  public:
    explicit GLContext(const OpenGLRendererConfig &config);
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
