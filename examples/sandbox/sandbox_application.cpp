#include "sandbox_application.h"

#include <nox/buffer.h>
#include <nox/command_list.h>
#include <nox/graphics_pipeline_state.h>
#include <nox/render_target.h>
#include <nox/shader.h>
#include <nox/swap_chain.h>
#include <nox/texture.h>
#include <nox/window.h>

namespace NOX {

namespace Shaders {

constexpr auto triangleVertexShaderSource = R"(
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

constexpr auto triangleFragmentShaderSource = R"(
            #version 460 core

            in vec3 color;
            out vec4 fragmentColor;

            void main() {
                fragmentColor = vec4(color, 1.0);
            }
        )";

} // namespace Shaders

void SandboxApplication::run() {
    CommandListDescriptor commandListDescriptor{};
    auto commandList = m_renderer->createCommandList(commandListDescriptor);
    commandList->setViewport(m_window->getSize());

    while (m_isRunning) {
        m_window->processEvents();

        m_triangleGraphicsPipelineState->bind();
        m_triangleVertexBuffer->bind();
        m_triangleIndexBuffer->bind();

        commandList->clearColor(Vector4D<float>{0.1f, 0.1f, 0.1f, 1.0f});
        commandList->drawIndexed(0u, 6u);
        commandList->drawIndexed(7u, 14u);

        m_swapChain->swap();
    }
}

SandboxApplication::SandboxApplication() {
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

    m_renderer = Renderer::create(RendererBackend::OPENGL);

    SwapChainDescriptor swapChainDescriptor{};
    swapChainDescriptor.isVSync = true;
    m_swapChain = m_renderer->createSwapChain(swapChainDescriptor, *m_window);
}

SandboxApplication::~SandboxApplication() = default;

void SandboxApplication::initialize() {
    createTriangleVertexBuffer();
}

void SandboxApplication::createTriangleVertexBuffer() {
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
    constexpr uint32_t indices[]{0u, 1u, 3u,
                                 1u, 2u, 3u,
                                 4u, 5u, 7u,
                                 5u, 6u, 7u};

    VertexFormat vertexFormat{
        {Format::RG32F},
        {Format::RGBA8_UNORM},
    };

    BufferDescriptor vertexBufferDescriptor{};
    vertexBufferDescriptor.usage = BufferUsage::STATIC;
    vertexBufferDescriptor.size = sizeof(vertices);
    vertexBufferDescriptor.data = vertices;
    m_triangleVertexBuffer = m_renderer->createVertexBuffer(vertexBufferDescriptor, vertexFormat);

    BufferDescriptor indexBufferDescriptor{};
    indexBufferDescriptor.usage = BufferUsage::STATIC;
    indexBufferDescriptor.size = sizeof(indices);
    indexBufferDescriptor.data = indices;
    m_triangleIndexBuffer = m_renderer->createIndexBuffer(indexBufferDescriptor, Format::R32UI);

    ShaderDescriptor vertexShaderDescriptor{};
    vertexShaderDescriptor.type = ShaderType::VERTEX;

    ShaderDescriptor fragmentShaderDescriptor{};
    fragmentShaderDescriptor.type = ShaderType::FRAGMENT;

    GraphicsPipelineStateDescriptor graphicsPipelineStateDescriptor{};
    graphicsPipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;
    graphicsPipelineStateDescriptor.shaderStages = {
        m_renderer->createShader(vertexShaderDescriptor, Shaders::triangleVertexShaderSource),
        m_renderer->createShader(fragmentShaderDescriptor, Shaders::triangleFragmentShaderSource),
    };

    m_triangleGraphicsPipelineState = m_renderer->createGraphicsPipelineState(graphicsPipelineStateDescriptor);
}

} // namespace NOX
