#include "sandbox_application.h"

#include <GLFW/glfw3.h>

#if defined(WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

namespace NOX {

namespace {

constexpr std::string_view windowTitle = "Sandbox Example";
constexpr Vector2D<int32_t> windowSize = {800, 600};

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

} // namespace

SandboxApplication::SandboxApplication() {
    auto result = glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(windowSize.x(), windowSize.y(), windowTitle.data(), nullptr, nullptr);

    m_renderer = Renderer::create(RendererBackend::OPENGL);

#if defined(WIN32)
    WindowsSurfaceBackendDescriptor surfaceBackendDescriptor{};
    surfaceBackendDescriptor.windowHandle = static_cast<void *>(glfwGetWin32Window(m_window));
#elif defined(__linux__)
    X11SurfaceBackendDescriptor surfaceBackendDescriptor{};
    surfaceBackendDescriptor.displayHandle = static_cast<void *>(glfwGetX11Display());
    surfaceBackendDescriptor.windowHandle = static_cast<uint64_t>(glfwGetX11Window(m_window));
#endif

    OpenGLSurfaceAttributesDescriptor surfaceAttributesDescriptor{};
    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = surfaceBackendDescriptor;
    surfaceDescriptor.surfaceAttributesDescriptor = surfaceAttributesDescriptor;
    m_surface = m_renderer->createSurface(surfaceDescriptor);

    SwapchainDescriptor swapchainDescriptor{};
    swapchainDescriptor.presentMode = OpenGLPresentMode{true};
    m_swapchain = m_renderer->createSwapchain(swapchainDescriptor);
}

SandboxApplication::~SandboxApplication() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
    m_window = nullptr;
}

void SandboxApplication::initialize() {
    createTriangleVertexBuffer();
}

void SandboxApplication::run() {
    CommandListDescriptor commandListDescriptor{};
    auto commandList = m_renderer->createCommandList(commandListDescriptor);
    commandList->setViewport(windowSize);

    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        m_triangleGraphicsPipelineState->bind();
        m_triangleVertexBuffer->bind();
        m_triangleIndexBuffer->bind();

        commandList->clearColor(Vector4D<float>{0.1f, 0.1f, 0.1f, 1.0f});
        commandList->drawIndexed(0u, 6u);
        commandList->drawIndexed(7u, 14u);

        m_swapchain->present();
    }
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
        Format::RG32F,
        Format::RGBA8_UNORM,
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
        m_renderer->createShader(vertexShaderDescriptor, triangleVertexShaderSource),
        m_renderer->createShader(fragmentShaderDescriptor, triangleFragmentShaderSource),
    };

    m_triangleGraphicsPipelineState = m_renderer->createGraphicsPipelineState(graphicsPipelineStateDescriptor);
}

} // namespace NOX
