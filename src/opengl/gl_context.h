#pragma once

#include <nox/surface.h>

#include <memory>

namespace nox {

class GLContext : public Surface {
  public:
    [[nodiscard]] static std::unique_ptr<GLContext> create(const SurfaceDescriptor &descriptor);

    virtual void swapBuffers() const = 0;
    virtual void setSwapInterval(bool interval) const = 0;

  public:
    GLContext();
    GLContext(const GLContext &) = delete;
    GLContext &operator=(const GLContext &) = delete;
    GLContext(GLContext &&) = delete;
    GLContext &operator=(GLContext &&) = delete;
    ~GLContext() override;

  protected:
    static uint8_t m_sContextCounter;
    static constexpr auto glMajorVersion = 4u;
    static constexpr auto glMinorVersion = 6u;
};

} // namespace nox
