#include <base/example.h>

#include <cstdint>

namespace nox {

namespace {

struct Vertex {
    float position[3];
    float color[3];
};
std::array<Vertex, 3> vertices{{
    {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
}};
std::array<uint32_t, 3> indices{0u, 1u, 2u};

constexpr std::string_view triangleVertexShaderSource = R"(
    #version 460 core

    layout(location = 0) in vec3 inPosition;
    layout(location = 1) in vec3 inColor;

    layout(location = 0) out vec3 outColor;

    out gl_PerVertex 
    {
        vec4 gl_Position;   
    };

    void main() 
    {
	    outColor = inColor;
	    gl_Position = vec4(inPosition, 1.0);
    }
)";

constexpr std::string_view triangleFragmentShaderSource = R"(
    #version 460 core

    layout(location = 0) in vec3 inColor;

    layout(location = 0) out vec4 outfragmentColor;

    void main() {
        outfragmentColor = vec4(inColor, 1.0);
    }
)";

} // namespace

class HelloTriangleExample : public base::Example {
  public:
    HelloTriangleExample() {
        constexpr std::string_view windowTitle = "Hello Triangle Example";
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

    void initialize() override {
        /*Setup vertex and index buffers*/
        {
            VertexBufferDescriptor vertexBufferDescriptor{};
            vertexBufferDescriptor.usage = BufferUsage::STATIC;
            vertexBufferDescriptor.size = static_cast<uint32_t>(vertices.size()) * sizeof(Vertex);
            vertexBufferDescriptor.data = vertices.data();
            vertexBufferDescriptor.vertexAttributes = {
                VertexAttributeFormat::RGB32F,
                VertexAttributeFormat::RGB32F,
            };
            m_vertexBuffer = m_renderer->createVertexBuffer(vertexBufferDescriptor);

            IndexBufferDescriptor indexBufferDescriptor{};
            indexBufferDescriptor.usage = BufferUsage::STATIC;
            indexBufferDescriptor.size = static_cast<uint32_t>(indices.size()) * sizeof(uint32_t);
            indexBufferDescriptor.data = indices.data();
            indexBufferDescriptor.format = VertexAttributeFormat::R32UI;
            m_indexBuffer = m_renderer->createIndexBuffer(indexBufferDescriptor);
        }

        /*Setup render pass*/
        {
            AttachmentDescriptor attachmentDescriptor{};
            attachmentDescriptor.format = m_swapchain->getSurfaceFormat();
            attachmentDescriptor.loadOp = AttachmentLoadOp::CLEAR;
            attachmentDescriptor.storeOp = AttachmentStoreOp::STORE;

            AttachmentReference attachmentReference{};
            attachmentReference.index = 0u;

            SubpassDescriptor subpassDescriptor{};
            subpassDescriptor.colorAttachmentReferences = {
                attachmentReference,
            };

            RenderPassDescriptor renderPassDescriptor{};
            renderPassDescriptor.attachmentDescriptors = {
                {attachmentDescriptor},
            };
            renderPassDescriptor.subpassDescriptors = {
                subpassDescriptor,
            };
            m_renderPass = m_renderer->createRenderPass(renderPassDescriptor);
        }

        /*Setup framebuffer*/
        {
            FramebufferDescriptor framebufferDescriptor{};
            framebufferDescriptor.attachments = m_swapchain->getSwapchainTextures();
            framebufferDescriptor.renderPass = m_renderPass.get();
            framebufferDescriptor.size = m_swapchain->getSize();
            m_framebuffer = m_renderer->createFramebuffer(framebufferDescriptor);
        }

        /*Setup pipeline layout*/
        {
            PipelineLayoutDescriptor pipelineLayoutDescriptor{};
            m_pipelineLayout = m_renderer->createPipelineLayout(pipelineLayoutDescriptor);
        }

        /*Setup graphics pipeline state*/
        {
            GraphicsPipelineStateDescriptor graphicsPipelineStateDescriptor{};

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

            graphicsPipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;
            graphicsPipelineStateDescriptor.subpassIndex = 0u;

            m_graphicsPipelineState = m_renderer->createGraphicsPipelineState(graphicsPipelineStateDescriptor);
        }
    }

    void onUpdate() override {
        RenderPassBeginDescriptor renderPassBeginDescriptor{};
        renderPassBeginDescriptor.renderPass = m_renderPass.get();
        renderPassBeginDescriptor.framebuffer = m_framebuffer.get();
        renderPassBeginDescriptor.clearValues = {
            {Vector4D<float>{0.0f, 0.0f, 0.0f, 1.0f}},
        };

        m_commandList->beginRenderPass(renderPassBeginDescriptor);
        {
            m_commandList->setViewport(m_swapchain->getSize());
            m_commandList->bindGraphicsPipelineState(*m_graphicsPipelineState);

            m_commandList->bindVertexBuffer(*m_vertexBuffer);
            m_commandList->bindIndexBuffer(*m_indexBuffer);

            m_commandList->drawIndexed(0u, 3u);
        }
        m_commandList->endRenderPass();
    }

  private:
    std::unique_ptr<CommandList> m_commandList{nullptr};
    std::unique_ptr<Buffer> m_vertexBuffer{nullptr};
    std::unique_ptr<Buffer> m_indexBuffer{nullptr};
    std::unique_ptr<RenderPass> m_renderPass{nullptr};
    std::unique_ptr<Framebuffer> m_framebuffer{nullptr};
    std::shared_ptr<PipelineLayout> m_pipelineLayout{nullptr};
    std::unique_ptr<GraphicsPipelineState> m_graphicsPipelineState{nullptr};
};

} // namespace nox

int main() {
    auto example = std::make_unique<nox::HelloTriangleExample>();
    example->initialize();
    example->run();

    return 0;
}
