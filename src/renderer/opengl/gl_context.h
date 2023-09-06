#pragma once

#include <memory>

namespace NOX {

struct PixelFormatDescriptor;

class Window;

class GLContext {
    static constexpr auto glMajorVersion = 4u;
    static constexpr auto glMinorVersion = 6u;

  public:
    GLContext();
    ~GLContext();

    void createExtendedContext(const PixelFormatDescriptor &descriptor, const Window &window);

    void makeCurrent() const;

    void swapBuffers() const;
    void setSwapInterval(bool value);

  public:
    GLContext(const GLContext &) = delete;
    GLContext &operator=(const GLContext &) = delete;
    GLContext(GLContext &&) = delete;
    GLContext &operator=(GLContext &&) = delete;

  private:
    struct Impl;
    std::unique_ptr<Impl> m_impl{nullptr};
};

} // namespace NOX
