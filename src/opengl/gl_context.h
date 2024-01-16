#pragma once

#include <nox/surface.h>

#include <memory>

namespace nox {

class GLContext : public Surface {
  public:
    [[nodiscard]] static bool validateInput(const SurfaceDescription &description);
    [[nodiscard]] static std::unique_ptr<GLContext> create(const SurfaceDescription &description);

    virtual void swapBuffers() const = 0;
    virtual void setSwapInterval(bool value) const = 0;

  public:
    GLContext() = default;
    GLContext(const GLContext &) = delete;
    GLContext &operator=(const GLContext &) = delete;
    GLContext(GLContext &&) = delete;
    GLContext &operator=(GLContext &&) = delete;

  protected:
    static constexpr auto glMajorVersion = 4u;
    static constexpr auto glMinorVersion = 6u;
};

} // namespace nox
