#include <nox/renderer/buffer.h>
#include <nox/renderer/command_list.h>
#include <nox/renderer/pipeline_state.h>
#include <nox/renderer/render_pass.h>
#include <nox/renderer/render_target.h>
#include <nox/renderer/renderer.h>
#include <nox/renderer/shader.h>
#include <nox/renderer/swap_chain.h>
#include <nox/renderer/texture.h>
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
    }

    void initialize() {
        initializeBuffers();
        createTriangleRenderPass();
        createFullscreenRenderPass();
    }

    void run() {
        CommandListDescriptor commandListDescriptor{};
        auto commandList = m_renderer->createCommandList(commandListDescriptor);
        commandList->setViewport(m_window->getSize());

        while (m_isRunning) {
            m_window->processEvents();

            commandList->beginRenderPass(*m_triangleRenderPass);
            {
                commandList->bindVertexBuffer(*m_vertexBuffer);
                commandList->bindIndexBuffer(*m_indexBuffer);

                commandList->clearColor(Vector4D<float>{0.1f, 0.1f, 0.1f, 1.0f});
                commandList->drawIndexed(0u, 6u);
                commandList->drawIndexed(7u, 14u);
            }
            commandList->endRenderPass();

            commandList->beginRenderPass(*m_fullscreenRenderPass);
            {
                commandList->bindVertexBuffer(*m_fullscreenVertexBuffer);
                commandList->bindIndexBuffer(*m_fullscreenIndexBuffer);

                commandList->clearColor(Vector4D<float>{0.1f, 0.1f, 0.1f, 1.0f});
                commandList->drawIndexed(0u, 6u);
            }
            commandList->endRenderPass();

            m_swapChain->swap();
        }
    }

  private:
    void initializeBuffers() {
        struct Vertex {
            float position[2];
            uint8_t color[4];
        };
        constexpr Vertex vertices[]{
            {{0.5f, 0.5f}, {255, 0, 0, 255}},
            {{0.5f, -0.5f}, {0, 255, 0, 255}},
            {{-0.5f, -0.5f}, {0, 0, 255, 255}},
            {{-0.5f, 0.5f}, {0, 255, 0, 255}},
            {{0.25f, 0.25f}, {255, 0, 0, 255}},
            {{0.25f, -0.25f}, {255, 0, 0, 255}},
            {{-0.25f, -0.25f}, {255, 0, 0, 255}},
            {{-0.25f, 0.25f}, {255, 0, 0, 255}},
        };
        constexpr uint32_t indices[]{0, 1, 3,
                                     1, 2, 3,
                                     4, 5, 7,
                                     5, 6, 7};

        VertexFormat vertexFormat;
        vertexFormat.attributes = {
            {"aPosition", Format::RG32F},
            {"aColor", Format::RGBA8_UNORM},
        };

        BufferDescriptor vertexBufferDescriptor{};
        vertexBufferDescriptor.accessMethod = BufferAccessMethod::STATIC;
        vertexBufferDescriptor.size = sizeof(vertices);
        vertexBufferDescriptor.data = vertices;
        m_vertexBuffer = m_renderer->createVertexBuffer(vertexBufferDescriptor, vertexFormat);

        BufferDescriptor indexBufferDescriptor{};
        indexBufferDescriptor.accessMethod = BufferAccessMethod::STATIC;
        indexBufferDescriptor.size = sizeof(indices);
        indexBufferDescriptor.data = indices;
        m_indexBuffer = m_renderer->createIndexBuffer(indexBufferDescriptor, Format::R32UI);

        struct FullscreenVertex {
            float position[2];
            float textureCoordinates[2];
        };
        constexpr FullscreenVertex vertices1[] = {{{-1.0f, -1.0f}, {0.0f, 0.0f}}, // bottom left
                                                  {{1.0f, -1.0f}, {1.0f, 0.0f}},  // bottom right
                                                  {{1.0f, 1.0f}, {1.0f, 1.0f}},   // top right
                                                  {{-1.0f, 1.0f}, {0.0f, 1.0f}}}; // top left
        constexpr uint32_t indices1[] = {0u, 1u, 2u,
                                         2u, 3u, 0u};

        VertexFormat fullscreenVertexFormat;
        fullscreenVertexFormat.attributes = {
            {"aPosition", Format::RG32F},
            {"aTextureCoordinates", Format::RG32F},
        };

        BufferDescriptor fullscreenVertexBufferDescriptor{};
        fullscreenVertexBufferDescriptor.accessMethod = BufferAccessMethod::STATIC;
        fullscreenVertexBufferDescriptor.size = sizeof(vertices1);
        fullscreenVertexBufferDescriptor.data = vertices1;
        m_fullscreenVertexBuffer = m_renderer->createVertexBuffer(fullscreenVertexBufferDescriptor, fullscreenVertexFormat);

        BufferDescriptor fullscreenIndexBufferDescriptor{};
        fullscreenIndexBufferDescriptor.accessMethod = BufferAccessMethod::STATIC;
        fullscreenIndexBufferDescriptor.size = sizeof(indices1);
        fullscreenIndexBufferDescriptor.data = indices1;
        m_fullscreenIndexBuffer = m_renderer->createIndexBuffer(fullscreenIndexBufferDescriptor, Format::R32UI);
    }

    void createTriangleRenderPass() {
        TextureDescriptor textureDescriptor{};
        textureDescriptor.type = TextureType::TEXTURE2D;
        textureDescriptor.format = Format::RGBA32F;
        textureDescriptor.size = {m_window->getSize().x(), m_window->getSize().y(), 0u};
        m_triangleTexture = m_renderer->createTexture(textureDescriptor);

        RenderTargetDescriptor renderTargetDescriptor{};
        renderTargetDescriptor.attachments.colorAttachments = {
            {m_triangleTexture},
        };
        renderTargetDescriptor.resolution = m_window->getSize();

        PipelineStateDescriptor pipelineStateDescriptor{};
        pipelineStateDescriptor.renderTarget = m_renderer->createRenderTarget(renderTargetDescriptor);
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

        RenderPassDescriptor renderPassDescriptor{};
        renderPassDescriptor.pipelineState = m_renderer->createPipelineState(pipelineStateDescriptor);
        m_triangleRenderPass = m_renderer->createRenderPass(renderPassDescriptor);
    }

    void createFullscreenRenderPass() {
        PipelineStateDescriptor pipelineStateDescriptor{};
        pipelineStateDescriptor.renderTarget = m_swapChain->getRenderTarget();
        pipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;

        constexpr auto vertexShaderSource = R"(
            #version 460 core

            out gl_PerVertex {
	            vec4 gl_Position;
            };

		    layout (location = 0) in vec2 aPosition;
		    layout (location = 1) in vec2 aTextureCoordinates;

		    out vec2 textureCoordinates;

		    void main() {
			    gl_Position = vec4(aPosition, 0.0, 1.0); 
			    textureCoordinates = aTextureCoordinates;
		    }
        )";
        ShaderDescriptor vertexShaderDescriptor{};
        vertexShaderDescriptor.type = ShaderType::VERTEX;
        pipelineStateDescriptor.vertexShader = m_renderer->createShaderFromString(vertexShaderDescriptor, vertexShaderSource);

        constexpr auto fragmentShaderSource = R"(
            #version 460 core

            layout(binding = 0) uniform sampler2D fullscreenTexture;

            in vec2 textureCoordinates;
            out vec4 fragmentColor;

            void main() {
                fragmentColor = texture(fullscreenTexture, textureCoordinates);
            }
        )";
        ShaderDescriptor fragmentShaderDescriptor{};
        fragmentShaderDescriptor.type = ShaderType::FRAGMENT;
        pipelineStateDescriptor.fragmentShader = m_renderer->createShaderFromString(fragmentShaderDescriptor, fragmentShaderSource);

        RenderPassDescriptor renderPassDescriptor{};
        renderPassDescriptor.pipelineState = m_renderer->createPipelineState(pipelineStateDescriptor);
        m_fullscreenRenderPass = m_renderer->createRenderPass(renderPassDescriptor);
    }

  private:
    bool m_isRunning{true};
    std::unique_ptr<Window> m_window{nullptr};
    std::unique_ptr<Renderer, RendererDeleter> m_renderer{nullptr};
    std::shared_ptr<SwapChain> m_swapChain{nullptr};
    std::unique_ptr<Buffer> m_vertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_indexBuffer{nullptr};
    std::unique_ptr<Buffer> m_fullscreenVertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_fullscreenIndexBuffer{nullptr};
    std::shared_ptr<Texture> m_triangleTexture{nullptr};
    std::unique_ptr<RenderPass> m_triangleRenderPass{nullptr};
    std::unique_ptr<RenderPass> m_fullscreenRenderPass{nullptr};
};

int main() {
    auto application = std::make_unique<SandboxApplication>();
    application->initialize();
    application->run();

    return 0;
}
