#pragma once

#include <base/window.h>
#include <nox/renderer.h>

namespace nox::base {

class Example {
  public:
    virtual ~Example() = default;

    virtual void initialize() = 0;
    virtual void onUpdate() = 0;

    void run();

  protected:
    Window m_window{};
    RendererPtr m_renderer{nullptr};
    std::unique_ptr<Swapchain> m_swapchain{nullptr};
};

} // namespace nox::base
