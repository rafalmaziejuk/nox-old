#include "sandbox_example.h"

namespace nox {

namespace {

constexpr auto triangleVertexShaderSource = R"(
            #version 460 core

            layout(location = 0) in vec2 aPosition;

            out gl_PerVertex {
	            vec4 gl_Position;
            };

            void main() {
                gl_Position = vec4(aPosition, 0.0, 1.0);
            }
        )";

constexpr auto triangleFragmentShaderSource = R"(
            #version 460 core

            out vec4 fragmentColor;

            void main() {
                fragmentColor = vec4(1.0);
            }
        )";

struct Vertex {
    float position[2];
};
constexpr Vertex vertices[]{
    {{0.5f, 0.5f}},   // top right
    {{0.5f, -0.5f}},  // bottom right
    {{-0.5f, -0.5f}}, // bottom left
    {{-0.5f, 0.5f}},  // top left
};

constexpr uint32_t indices[]{0, 1, 3,
                             1, 2, 3};

} // namespace

SandboxExample::SandboxExample() {
    constexpr std::string_view windowTitle = "Sandbox Example";
    constexpr Vector2D<uint32_t> windowSize = {800u, 600u};
    m_window.initialize(windowTitle, windowSize);

    m_renderer = Renderer::create(RendererBackend::OPENGL);

    OpenGLSurfaceAttributesDescriptor surfaceAttributesDescriptor{};
    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = m_window.getSurfaceBackendDescriptor();
    surfaceDescriptor.surfaceAttributesDescriptor = surfaceAttributesDescriptor;

    SwapchainDescriptor swapchainDescriptor{};
    swapchainDescriptor.surfaceDescriptor = surfaceDescriptor;
    swapchainDescriptor.size = m_window.getSize();
    swapchainDescriptor.presentMode = OpenGLPresentMode{true};
    m_swapchain = m_renderer->createSwapchain(swapchainDescriptor);

    CommandListDescriptor commandListDescriptor{};
    m_commandList = m_renderer->createCommandList(commandListDescriptor);
}

void SandboxExample::initialize() {
    createBuffer();
    createRenderPass();
    createFramebuffer();
    createGraphicsPipelineState();
}

void SandboxExample::onUpdate() {
    RenderPassBeginDescriptor renderPassBeginDescriptor{};
    renderPassBeginDescriptor.renderPass = m_renderPass.get();
    renderPassBeginDescriptor.framebuffer = m_framebuffer.get();
    renderPassBeginDescriptor.clearValues = {
        {Vector4D<float>{0.1f, 0.1f, 0.1f, 1.0f}},
    };

    m_commandList->beginRenderPass(renderPassBeginDescriptor);
    {
        m_graphicsPipelineState->bind();
        m_commandList->setViewport(m_swapchain->getSize());

        m_vertexBuffer->bind();
        m_indexBuffer->bind();

        m_commandList->drawIndexed(0u, 6u);
    }
    m_commandList->endRenderPass();
}

void SandboxExample::createBuffer() {
    VertexBufferDescriptor vertexBufferDescriptor{};
    vertexBufferDescriptor.usage = BufferUsage::STATIC;
    vertexBufferDescriptor.size = sizeof(vertices);
    vertexBufferDescriptor.data = vertices;
    vertexBufferDescriptor.vertexAttributes = {
        VertexAttributeFormat::RG32F,
    };
    m_vertexBuffer = m_renderer->createVertexBuffer(vertexBufferDescriptor);

    IndexBufferDescriptor indexBufferDescriptor{};
    indexBufferDescriptor.usage = BufferUsage::STATIC;
    indexBufferDescriptor.size = sizeof(indices);
    indexBufferDescriptor.data = indices;
    indexBufferDescriptor.format = VertexAttributeFormat::R32UI;
    m_indexBuffer = m_renderer->createIndexBuffer(indexBufferDescriptor);
}

void SandboxExample::createRenderPass() {
    AttachmentDescriptor attachmentDescriptor{};
    attachmentDescriptor.format = m_swapchain->getSurfaceFormat();
    attachmentDescriptor.loadOp = AttachmentLoadOp::CLEAR;
    attachmentDescriptor.storeOp = AttachmentStoreOp::STORE;

    RenderPassDescriptor renderPassDescriptor{};
    renderPassDescriptor.attachmentsDescriptors = {
        {attachmentDescriptor},
    };
    m_renderPass = m_renderer->createRenderPass(renderPassDescriptor);
}

void SandboxExample::createFramebuffer() {
    FramebufferDescriptor framebufferDescriptor{};
    framebufferDescriptor.attachments = m_swapchain->getPresentableTextures();
    framebufferDescriptor.renderPass = m_renderPass.get();
    framebufferDescriptor.size = m_swapchain->getSize();
    m_framebuffer = m_renderer->createFramebuffer(framebufferDescriptor);
}

void SandboxExample::createGraphicsPipelineState() {
    GraphicsPipelineStateDescriptor graphicsPipelineStateDescriptor{};
    graphicsPipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;

    PipelineLayoutDescriptor pipelineLayoutDescriptor{};
    graphicsPipelineStateDescriptor.pipelineLayout = m_renderer->createPipelineLayout(pipelineLayoutDescriptor);

    ShaderDescriptor vertexShaderDescriptor{};
    vertexShaderDescriptor.type = ShaderType::VERTEX;
    auto vertexShader = m_renderer->createShader(vertexShaderDescriptor, triangleVertexShaderSource);

    ShaderDescriptor fragmentShaderDescriptor{};
    fragmentShaderDescriptor.type = ShaderType::FRAGMENT;
    auto fragmentShader = m_renderer->createShader(fragmentShaderDescriptor, triangleFragmentShaderSource);

    graphicsPipelineStateDescriptor.shaderStages = {
        vertexShader.get(),
        fragmentShader.get(),
    };
    m_graphicsPipelineState = m_renderer->createGraphicsPipelineState(graphicsPipelineStateDescriptor);
}

} // namespace nox
