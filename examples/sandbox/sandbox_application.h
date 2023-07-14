#pragma once

#include <nox/renderer.h>

#include <memory>

namespace NOX {

class Buffer;
class RenderPass;
class SwapChain;
class Texture;
class Window;

class SandboxApplication {
  public:
    SandboxApplication();
    ~SandboxApplication();

    void initialize();
    void run();

  private:
    void createTriangleVertexBuffer();
    void createTriangleRenderPass();

    void createFullscreenVertexBuffer();
    void createFullscreenRenderPass();

  private:
    bool m_isRunning{true};
    std::unique_ptr<Window> m_window{nullptr};
    std::unique_ptr<Renderer, RendererDeleter> m_renderer{nullptr};
    std::shared_ptr<SwapChain> m_swapChain{nullptr};
    std::unique_ptr<Buffer> m_triangleVertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_triangleIndexBuffer{nullptr};
    std::unique_ptr<RenderPass> m_triangleRenderPass{nullptr};
    std::unique_ptr<Buffer> m_fullscreenVertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_fullscreenIndexBuffer{nullptr};
    std::unique_ptr<RenderPass> m_fullscreenRenderPass{nullptr};
    std::shared_ptr<Texture> m_outputTexture{nullptr};
};

} // namespace NOX
