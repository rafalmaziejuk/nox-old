#include "sandbox_application.h"

#include <nox/buffer.h>
#include <nox/command_list.h>
#include <nox/pipeline_state.h>
#include <nox/render_pass.h>
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

constexpr auto fullscreenVertexShaderSource = R"(
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

constexpr auto fullscreenFragmentShaderSource = R"(
            #version 460 core

            layout(binding = 0) uniform sampler2D fullscreenTexture;

            in vec2 textureCoordinates;
            out vec4 fragmentColor;

            void main() {
                fragmentColor = texture(fullscreenTexture, textureCoordinates);
            }
        )";

} // namespace Shaders

void SandboxApplication::run() {
    CommandListDescriptor commandListDescriptor{};
    auto commandList = m_renderer->createCommandList(commandListDescriptor);
    commandList->setViewport(m_window->getSize());

    while (m_isRunning) {
        m_window->processEvents();

        commandList->beginRenderPass(*m_triangleRenderPass);
        {
            commandList->bindVertexBuffer(*m_triangleVertexBuffer);
            commandList->bindIndexBuffer(*m_triangleIndexBuffer);

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

    RendererDescriptor rendererDescriptor;
    rendererDescriptor.api = RendererAPI::OPENGL;
    m_renderer = Renderer::create(rendererDescriptor);

    SwapChainDescriptor swapChainDescriptor{};
    swapChainDescriptor.isVSync = true;
    m_swapChain = m_renderer->createSwapChain(swapChainDescriptor, *m_window);
}

SandboxApplication::~SandboxApplication() = default;

void SandboxApplication::initialize() {
    createTriangleVertexBuffer();
    createTriangleRenderPass();
    createFullscreenVertexBuffer();
    createFullscreenRenderPass();
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

    VertexFormat vertexFormat;
    vertexFormat.attributes = {
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
}

void SandboxApplication::createTriangleRenderPass() {
    TextureDescriptor textureDescriptor{};
    textureDescriptor.type = TextureType::TEXTURE2D;
    textureDescriptor.format = Format::RGBA32F;
    textureDescriptor.size = {m_window->getSize().x(), m_window->getSize().y(), 0u};
    m_outputTexture = m_renderer->createTexture(textureDescriptor);

    RenderTargetDescriptor renderTargetDescriptor{};
    renderTargetDescriptor.attachments.colorAttachments = {
        {m_outputTexture},
    };
    renderTargetDescriptor.size = m_window->getSize();

    PipelineStateDescriptor pipelineStateDescriptor{};
    pipelineStateDescriptor.renderTarget = m_renderer->createRenderTarget(renderTargetDescriptor);
    pipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;

    ShaderDescriptor vertexShaderDescriptor{};
    vertexShaderDescriptor.stage = ShaderStage::VERTEX;
    pipelineStateDescriptor.vertexShader = m_renderer->createShaderFromString(vertexShaderDescriptor, Shaders::triangleVertexShaderSource);

    ShaderDescriptor fragmentShaderDescriptor{};
    fragmentShaderDescriptor.stage = ShaderStage::FRAGMENT;
    pipelineStateDescriptor.fragmentShader = m_renderer->createShaderFromString(fragmentShaderDescriptor, Shaders::triangleFragmentShaderSource);

    RenderPassDescriptor renderPassDescriptor{};
    renderPassDescriptor.pipelineState = m_renderer->createPipelineState(pipelineStateDescriptor);
    m_triangleRenderPass = m_renderer->createRenderPass(renderPassDescriptor);
}

void SandboxApplication::createFullscreenVertexBuffer() {
    struct Vertex {
        float position[2];
        float textureCoordinates[2];
    };
    constexpr Vertex vertices[] = {{{-1.0f, -1.0f}, {0.0f, 0.0f}}, // bottom left
                                   {{1.0f, -1.0f}, {1.0f, 0.0f}},  // bottom right
                                   {{1.0f, 1.0f}, {1.0f, 1.0f}},   // top right
                                   {{-1.0f, 1.0f}, {0.0f, 1.0f}}}; // top left
    constexpr uint32_t indices[] = {0u, 1u, 2u,
                                    2u, 3u, 0u};

    VertexFormat vertexFormat;
    vertexFormat.attributes = {
        {Format::RG32F},
        {Format::RG32F},
    };

    BufferDescriptor vertexBufferDescriptor{};
    vertexBufferDescriptor.usage = BufferUsage::STATIC;
    vertexBufferDescriptor.size = sizeof(vertices);
    vertexBufferDescriptor.data = vertices;
    m_fullscreenVertexBuffer = m_renderer->createVertexBuffer(vertexBufferDescriptor, vertexFormat);

    BufferDescriptor indexBufferDescriptor{};
    indexBufferDescriptor.usage = BufferUsage::STATIC;
    indexBufferDescriptor.size = sizeof(indices);
    indexBufferDescriptor.data = indices;
    m_fullscreenIndexBuffer = m_renderer->createIndexBuffer(indexBufferDescriptor, Format::R32UI);
}

void SandboxApplication::createFullscreenRenderPass() {
    PipelineStateDescriptor pipelineStateDescriptor{};
    pipelineStateDescriptor.renderTarget = m_swapChain->getRenderTarget();
    pipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;

    ShaderDescriptor vertexShaderDescriptor{};
    vertexShaderDescriptor.stage = ShaderStage::VERTEX;
    pipelineStateDescriptor.vertexShader = m_renderer->createShaderFromString(vertexShaderDescriptor, Shaders::fullscreenVertexShaderSource);

    ShaderDescriptor fragmentShaderDescriptor{};
    fragmentShaderDescriptor.stage = ShaderStage::FRAGMENT;
    pipelineStateDescriptor.fragmentShader = m_renderer->createShaderFromString(fragmentShaderDescriptor, Shaders::fullscreenFragmentShaderSource);

    auto &bindingDescriptors = pipelineStateDescriptor.pipelineLayoutDescriptor.bindingDescriptors;
    BindingDescriptor outputTextureBindingDescriptor{};
    outputTextureBindingDescriptor.set = 0u;
    outputTextureBindingDescriptor.binding = 0u;
    outputTextureBindingDescriptor.shaderStages = ShaderStage::FRAGMENT;
    outputTextureBindingDescriptor.resourceType = ResourceType::TEXTURE;
    bindingDescriptors.push_back(outputTextureBindingDescriptor);

    RenderPassDescriptor renderPassDescriptor{};
    renderPassDescriptor.pipelineState = m_renderer->createPipelineState(pipelineStateDescriptor);
    m_fullscreenRenderPass = m_renderer->createRenderPass(renderPassDescriptor);
    m_fullscreenRenderPass->setInput(outputTextureBindingDescriptor.binding, *m_outputTexture);
}

} // namespace NOX
