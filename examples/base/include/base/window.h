#pragma once

#include <nox/surface.h>
#include <nox/vector.h>

#include <string_view>

struct GLFWwindow;

namespace nox::base {

class Window {
  public:
    ~Window();

    void initialize(std::string_view title, const Vector2D<int32_t> &size);

    [[nodiscard]] bool shouldClose() const;
    void processEvents() const;

    [[nodiscard]] SurfaceBackendDescriptor getSurfaceBackendDescriptor() const;
    [[nodiscard]] Vector2D<int32_t> getSize() const;

  private:
    GLFWwindow *m_handle{nullptr};
    Vector2D<int32_t> m_size;
};

} // namespace nox::base
