#include <nox/renderer/buffer.h>
#include <nox/renderer/renderer.h>
#include <nox/renderer/swap_chain.h>
#include <nox/window/event.h>
#include <nox/window/window.h>

using namespace NOX;

class SandboxApplication {
  public:
    SandboxApplication() {
        WindowDescriptor windowDescriptor{};
        windowDescriptor.title = "Sandbox Example";
        windowDescriptor.size = {800u, 600u};
        windowDescriptor.isVisible = true;
        windowDescriptor.isCentered = true;
        windowDescriptor.isResizable = true;
        m_window = Window::create(windowDescriptor);

        EventDispatcher eventDispatcher;
        eventDispatcher.closeEventCallback = [this]() {
            m_isRunning = false;
            return true;
        };
        m_window->pushBackEventDispatcher(eventDispatcher);

        OpenGLRendererConfig rendererConfig;
        rendererConfig.majorVersion = 4;
        rendererConfig.minorVersion = 6;
        RendererDescriptor rendererDescriptor;
        rendererDescriptor.api = RendererAPI::OPENGL;
        rendererDescriptor.config = rendererConfig;
        m_renderer = Renderer::create(rendererDescriptor);

        SwapChainDescriptor swapChainDescriptor{};
        swapChainDescriptor.isVSync = true;
        m_swapChain = m_renderer->createSwapChain(swapChainDescriptor, *m_window);

        struct Vertex {
            float position[2];
            uint8_t color[4];
        };
        Vertex vertices[]{
            {{0.5f, 0.5f}, {255, 0, 0, 255}},   // 1st vertex: center-top, red
            {{0.5f, -0.5f}, {0, 255, 0, 255}},  // 2nd vertex: right-bottom, green
            {{-0.5f, -0.5f}, {0, 0, 255, 255}}, // 3rd vertex: left-bottom, blue
            {{-0.5f, 0.5f}, {0, 255, 0, 255}},  // 3rd vertex: left-bottom, blue
            {{0.25f, 0.25f}, {0, 0, 0, 255}},   // 1st vertex: center-top, red
            {{0.25f, -0.25f}, {0, 0, 0, 255}},  // 2nd vertex: right-bottom, green
            {{-0.25f, -0.25f}, {0, 0, 0, 255}}, // 3rd vertex: left-bottom, blue
            {{-0.25f, 0.25f}, {0, 0, 0, 255}},  // 3rd vertex: left-bottom, blue
        };
        uint32_t indices[] = {
            // note that we start from 0!
            0, 1, 3, // first Triangle
            1, 2, 3, // second Triangle
            4, 5, 7,
            5, 6, 7};

        VertexFormat vertexFormat;
        vertexFormat.attributes.reserve(2);
        vertexFormat.attributes.push_back({"aPosition", Format::RG32_FLOAT});
        vertexFormat.attributes.push_back({"aColor", Format::RGBA8_UINT_NORM});

        BufferDescriptor vertexBufferDescriptor{};
        vertexBufferDescriptor.accessMethod = BufferAccessMethod::STATIC;
        vertexBufferDescriptor.size = sizeof(vertices);
        vertexBufferDescriptor.data = vertices;
        m_vertexBuffer = m_renderer->createVertexBuffer(vertexBufferDescriptor, vertexFormat);

        BufferDescriptor indexBufferDescriptor{};
        indexBufferDescriptor.accessMethod = BufferAccessMethod::STATIC;
        indexBufferDescriptor.size = sizeof(indices);
        indexBufferDescriptor.data = indices;
        m_indexBuffer = m_renderer->createIndexBuffer(indexBufferDescriptor, Format::R32_UINT);
    }

    void run() {
        while (m_isRunning) {
            m_window->processEvents();
            m_swapChain->swap();
        }
    }

  private:
    bool m_isRunning{true};
    std::unique_ptr<Window> m_window{nullptr};
    std::unique_ptr<Renderer, RendererDeleter> m_renderer{nullptr};
    std::unique_ptr<SwapChain> m_swapChain{nullptr};
    std::unique_ptr<Buffer> m_vertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_indexBuffer{nullptr};
};

int main() {
    auto application = std::make_unique<SandboxApplication>();
    application->run();

    return 0;
}
