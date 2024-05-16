#pragma once

#include <nox/surface.h>
#include <nox/vector.h>

struct GLFWwindow;

namespace nox::test {

class Window {
  public:
    Window();
    ~Window();

    [[nodiscard]] SurfaceBackendDescriptor getSurfaceBackendDescriptor() const;
    [[nodiscard]] Vector2D<uint32_t> getSize() const;

  private:
    GLFWwindow *m_handle{nullptr};
    Vector2D<uint32_t> m_size{};
};

} // namespace nox::test
