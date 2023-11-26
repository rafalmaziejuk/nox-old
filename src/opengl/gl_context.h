#pragma once

#include <memory>

namespace NOX {

struct PixelFormatDescriptor;

class GLContext {
    static constexpr auto glMajorVersion = 4u;
    static constexpr auto glMinorVersion = 6u;

  public:
    GLContext();
    ~GLContext();

    void createExtendedContext(const PixelFormatDescriptor &descriptor);

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

class GLWithContext {
  public:
    explicit GLWithContext(GLContext &context) : m_context{&context} {}
    virtual ~GLWithContext() = default;

    GLContext &getContext() const { return *m_context; }

  public:
    GLWithContext(const GLWithContext &) = delete;
    GLWithContext &operator=(const GLWithContext &) = delete;

  private:
    GLContext *m_context{nullptr};
};

} // namespace NOX
