#include "sandbox_example.h"

namespace nox {

namespace {

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

} // namespace

void SandboxExample::initialize() {
    constexpr std::string_view windowTitle = "Sandbox Example";
    constexpr Vector2D<int32_t> windowSize = {800, 600};
    m_window.initialize(windowTitle, windowSize);

    m_renderer = Renderer::create(RendererBackend::OPENGL);

    OpenGLSurfaceAttributesDescriptor surfaceAttributesDescriptor{};
    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = m_window.getSurfaceBackendDescriptor();
    surfaceDescriptor.surfaceAttributesDescriptor = surfaceAttributesDescriptor;

    SwapchainDescriptor swapchainDescriptor{};
    swapchainDescriptor.surfaceDescriptor = surfaceDescriptor;
    swapchainDescriptor.presentMode = OpenGLPresentMode{true};
    m_swapchain = m_renderer->createSwapchain(swapchainDescriptor);

    CommandListDescriptor commandListDescriptor{};
    m_commandList = m_renderer->createCommandList(commandListDescriptor);
    m_commandList->setViewport(m_window.getSize());

    ShaderDescriptor vertexShaderDescriptor{};
    vertexShaderDescriptor.type = ShaderType::VERTEX;

    ShaderDescriptor fragmentShaderDescriptor{};
    fragmentShaderDescriptor.type = ShaderType::FRAGMENT;

    GraphicsPipelineStateDescriptor graphicsPipelineStateDescriptor{};
    graphicsPipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;
    graphicsPipelineStateDescriptor.shaderStages = {
        m_renderer->createShader(vertexShaderDescriptor, triangleVertexShaderSource),
        m_renderer->createShader(fragmentShaderDescriptor, triangleFragmentShaderSource),
    };
    m_graphicsPipelineState = m_renderer->createGraphicsPipelineState(graphicsPipelineStateDescriptor);

    VertexBufferDescriptor vertexBufferDescriptor{};
    vertexBufferDescriptor.usage = BufferUsage::STATIC;
    vertexBufferDescriptor.size = sizeof(vertices);
    vertexBufferDescriptor.data = vertices;
    vertexBufferDescriptor.vertexFormat = {
        Format::RG32F,
        Format::RGBA8_UNORM,
    };
    m_vertexBuffer = m_renderer->createVertexBuffer(vertexBufferDescriptor);

    IndexBufferDescriptor indexBufferDescriptor{};
    indexBufferDescriptor.usage = BufferUsage::STATIC;
    indexBufferDescriptor.size = sizeof(indices);
    indexBufferDescriptor.data = indices;
    indexBufferDescriptor.format = Format::R32UI;
    m_indexBuffer = m_renderer->createIndexBuffer(indexBufferDescriptor);
}

void SandboxExample::onUpdate() {
    m_graphicsPipelineState->bind();
    m_vertexBuffer->bind();
    m_indexBuffer->bind();

    m_commandList->clearColor(Vector4D<float>{0.1f, 0.1f, 0.1f, 1.0f});
    m_commandList->drawIndexed(0u, 6u);
    m_commandList->drawIndexed(7u, 14u);
}

} // namespace nox
