#include <nox/renderer/buffer.h>
#include <nox/renderer/command_list.h>
#include <nox/renderer/pipeline_state.h>
#include <nox/renderer/renderer.h>
#include <nox/renderer/shader.h>
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
            {{0.5f, 0.5f}, {255, 0, 0, 255}},
            {{0.5f, -0.5f}, {0, 255, 0, 255}},
            {{-0.5f, -0.5f}, {0, 0, 255, 255}},
            {{-0.5f, 0.5f}, {0, 255, 0, 255}},
            {{0.25f, 0.25f}, {0, 0, 0, 255}},
            {{0.25f, -0.25f}, {0, 0, 0, 255}},
            {{-0.25f, -0.25f}, {0, 0, 0, 255}},
            {{-0.25f, 0.25f}, {0, 0, 0, 255}},
        };
        uint32_t indices[]{0, 1, 3,
                           1, 2, 3,
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

        PipelineStateDescriptor pipelineStateDescriptor{};
        pipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;

        constexpr auto vertexShaderSource = R"(
            #version 460 core

            layout(location = 0) in vec2 aPosition;
            layout(location = 1) in vec3 aColor;

            out gl_PerVertex {
	            vec4 gl_Position;
            };

            out vec3 color;

            void main() {
                gl_Position = vec4(aPosition, 0.0, 1.0);
                color = aColor;
            }
        )";
        ShaderDescriptor vertexShaderDescriptor{};
        vertexShaderDescriptor.type = ShaderType::VERTEX;
        pipelineStateDescriptor.vertexShader = m_renderer->createShaderFromString(vertexShaderDescriptor, vertexShaderSource);

        constexpr auto fragmentShaderSource = R"(
            #version 460 core

            in vec3 color;
            out vec4 fragmentColor;

            void main() {
                fragmentColor = vec4(color, 1.0);
            }
        )";
        ShaderDescriptor fragmentShaderDescriptor{};
        fragmentShaderDescriptor.type = ShaderType::FRAGMENT;
        pipelineStateDescriptor.fragmentShader = m_renderer->createShaderFromString(fragmentShaderDescriptor, fragmentShaderSource);

        m_pipelineState = m_renderer->createPipelineState(pipelineStateDescriptor);
    }

    void run() {
        CommandListDescriptor commandListDescriptor{};
        auto commandList = m_renderer->createCommandList(commandListDescriptor);

        commandList->bindVertexBuffer(*m_vertexBuffer);
        commandList->bindIndexBuffer(*m_indexBuffer);
        commandList->bindPipelineState(*m_pipelineState);

        commandList->setViewport(m_window->getSize());
        commandList->setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

        while (m_isRunning) {
            m_window->processEvents();

            commandList->clear(ClearFlag::COLOR);
            commandList->drawIndexed(0u, 6u);
            commandList->drawIndexed(7u, 14u);

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
    std::unique_ptr<PipelineState> m_pipelineState{nullptr};
};

int main() {
    auto application = std::make_unique<SandboxApplication>();
    application->run();

    return 0;
}
