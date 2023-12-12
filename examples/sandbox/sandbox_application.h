#pragma once

#include <nox/renderer.h>

struct GLFWwindow;

namespace nox {

class SandboxApplication {
  public:
    SandboxApplication();

    void initialize();
    void run();

  private:
    void createTriangleVertexBuffer();

  private:
    GLFWwindow *m_window{nullptr};
    RendererPtr m_renderer{nullptr};
    std::unique_ptr<Swapchain> m_swapchain{nullptr};
    std::unique_ptr<GraphicsPipelineState> m_triangleGraphicsPipelineState{nullptr};
    std::unique_ptr<Buffer> m_triangleVertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_triangleIndexBuffer{nullptr};
};

} // namespace nox
