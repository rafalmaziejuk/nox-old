#pragma once

#include <nox/renderer.h>

namespace NOX {

class SandboxApplication {
  public:
    SandboxApplication();
    ~SandboxApplication();

    void initialize();
    void run();

  private:
    void createTriangleVertexBuffer();

  private:
    bool m_isRunning{true};
    RendererPtr m_renderer{nullptr};
    std::unique_ptr<Window> m_window{nullptr};
    std::shared_ptr<SwapChain> m_swapChain{nullptr};
    std::unique_ptr<GraphicsPipelineState> m_triangleGraphicsPipelineState{nullptr};
    std::unique_ptr<Buffer> m_triangleVertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_triangleIndexBuffer{nullptr};
};

} // namespace NOX
