#include "sandbox_example.h"

#include <base/image.h>

namespace nox {

namespace {

constexpr auto triangleVertexShaderSource = R"(
            #version 460 core

            layout(location = 0) in vec2 aPosition;
            layout(location = 1) in vec2 aTextureCoordinates;

            out gl_PerVertex {
	            vec4 gl_Position;
            };

            out vec2 textureCoordinates;

            void main() {
                gl_Position = vec4(aPosition, 0.0, 1.0);
                textureCoordinates = aTextureCoordinates;
            }
        )";

constexpr auto triangleFragmentShaderSource = R"(
            #version 460 core

            layout(binding = 0) uniform sampler2D uTexture;

            in vec2 textureCoordinates;
            out vec4 fragmentColor;

            void main() {
                fragmentColor = texture(uTexture, textureCoordinates);
            }
        )";

struct Vertex {
    float position[2];
    float textureCoordinates[2];
};
constexpr Vertex vertices[]{
    {{0.5f, 0.5f}, {1.0f, 1.0f}},   // top right
    {{0.5f, -0.5f}, {1.0f, 0.0f}},  // bottom right
    {{-0.5f, -0.5f}, {0.0f, 0.0f}}, // bottom left
    {{-0.5f, 0.5f}, {0.0f, 1.0f}},  // top left
};

constexpr uint32_t indices[]{0, 1, 3,
                             1, 2, 3};

} // namespace

SandboxExample::SandboxExample() {
    constexpr std::string_view windowTitle = "Sandbox Example";
    constexpr Vector2D<uint32_t> windowSize = {800u, 600u};
    m_window.initialize(windowTitle, windowSize);

    m_renderer = Renderer::create(RendererBackend::OPENGL);

    OpenGLSurfaceAttributesDescription surfaceAttributesDescription{};
    SurfaceDescription surfaceDescription{};
    surfaceDescription.surfaceBackendDescription = m_window.getSurfaceBackendDescription();
    surfaceDescription.surfaceAttributesDescription = surfaceAttributesDescription;

    SwapchainDescription swapchainDescription{};
    swapchainDescription.surfaceDescription = surfaceDescription;
    swapchainDescription.size = m_window.getSize();
    swapchainDescription.presentMode = OpenGLPresentMode{true};
    m_swapchain = m_renderer->createSwapchain(swapchainDescription);

    CommandListDescription commandListDescription{};
    m_commandList = m_renderer->createCommandList(commandListDescription);
}

void SandboxExample::initialize() {
    createBuffer();
    createTexture();
    createRenderPass();
    createFramebuffer();
    createGraphicsPipelineState();
}

void SandboxExample::onUpdate() {
    RenderPassBeginDescription renderPassBeginDescription{};
    renderPassBeginDescription.renderPass = m_renderPass.get();
    renderPassBeginDescription.framebuffer = m_framebuffer.get();
    renderPassBeginDescription.clearValues = {
        {Vector4D<float>{0.1f, 0.1f, 0.1f, 1.0f}},
    };

    m_commandList->beginRenderPass(renderPassBeginDescription);
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
    VertexBufferDescription vertexBufferDescription{};
    vertexBufferDescription.usage = BufferUsage::STATIC;
    vertexBufferDescription.size = sizeof(vertices);
    vertexBufferDescription.data = vertices;
    vertexBufferDescription.vertexAttributes = {
        VertexAttributeFormat::RG32F,
        VertexAttributeFormat::RG32F,
    };
    m_vertexBuffer = m_renderer->createVertexBuffer(vertexBufferDescription);

    IndexBufferDescription indexBufferDescription{};
    indexBufferDescription.usage = BufferUsage::STATIC;
    indexBufferDescription.size = sizeof(indices);
    indexBufferDescription.data = indices;
    indexBufferDescription.format = VertexAttributeFormat::R32UI;
    m_indexBuffer = m_renderer->createIndexBuffer(indexBufferDescription);
}

void SandboxExample::createTexture() {
    base::Image containerImage("assets/container.jpg");

    Texture2DDescription textureDescription{};
    textureDescription.format = ImageFormat::RGB8_UNORM;
    textureDescription.size = containerImage.size;
    m_texture = m_renderer->createTexture2D(textureDescription);

    TextureWriteDescription textureWriteDescription{};
    textureWriteDescription.data = containerImage.data;
    textureWriteDescription.dataFormat = ImageFormat::RGB8UI;
    textureWriteDescription.subresource.baseMipmapLevel = 0u;
    textureWriteDescription.size = {containerImage.size.x(), containerImage.size.y(), 0u};
    m_texture->write(textureWriteDescription);
}

void SandboxExample::createRenderPass() {
    AttachmentDescription attachmentDescription{};
    attachmentDescription.format = m_swapchain->getSurfaceFormat();
    attachmentDescription.loadOp = AttachmentLoadOp::CLEAR;
    attachmentDescription.storeOp = AttachmentStoreOp::STORE;

    RenderPassDescription renderPassDescription{};
    renderPassDescription.attachmentsDescriptions = {
        {attachmentDescription},
    };
    m_renderPass = m_renderer->createRenderPass(renderPassDescription);
}

void SandboxExample::createFramebuffer() {
    FramebufferDescription framebufferDescription{};
    framebufferDescription.attachments = m_swapchain->getPresentableTextures();
    framebufferDescription.renderPass = m_renderPass.get();
    framebufferDescription.size = m_swapchain->getSize();
    m_framebuffer = m_renderer->createFramebuffer(framebufferDescription);
}

void SandboxExample::createGraphicsPipelineState() {
    GraphicsPipelineStateDescription graphicsPipelineStateDescription{};
    graphicsPipelineStateDescription.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;

    DescriptionSetLayoutBinding descriptionSetLayoutBinding{};
    descriptionSetLayoutBinding.bindingIndex = 0u;
    descriptionSetLayoutBinding.resourceType = ResourceType::TEXTURE;
    descriptionSetLayoutBinding.textureResourceDescriptions = {
        {m_texture},
    };

    DescriptionSetLayout descriptionSetLayout{};
    descriptionSetLayout.bindings = {
        descriptionSetLayoutBinding,
    };

    PipelineLayoutDescription pipelineLayoutDescription{};
    pipelineLayoutDescription.setLayouts = {
        descriptionSetLayout,
    };
    graphicsPipelineStateDescription.pipelineLayout = m_renderer->createPipelineLayout(pipelineLayoutDescription);

    ShaderDescription vertexShaderDescription{};
    vertexShaderDescription.type = ShaderType::VERTEX;
    auto vertexShader = m_renderer->createShader(vertexShaderDescription, triangleVertexShaderSource);

    ShaderDescription fragmentShaderDescription{};
    fragmentShaderDescription.type = ShaderType::FRAGMENT;
    auto fragmentShader = m_renderer->createShader(fragmentShaderDescription, triangleFragmentShaderSource);

    graphicsPipelineStateDescription.shaderStages = {
        vertexShader.get(),
        fragmentShader.get(),
    };

    m_graphicsPipelineState = m_renderer->createGraphicsPipelineState(graphicsPipelineStateDescription);
}

} // namespace nox
