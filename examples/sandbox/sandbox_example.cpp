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
    createTexture();
    createRenderPass();
    createFramebuffer();
    createGraphicsPipelineState();
}

void SandboxExample::onUpdate() {
    RenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.renderPass = m_renderPass.get();
    renderPassBeginInfo.framebuffer = m_framebuffer.get();
    renderPassBeginInfo.clearValues = {
        {Vector4D<float>{0.1f, 0.1f, 0.1f, 1.0f}},
    };

    m_commandList->beginRenderPass(renderPassBeginInfo);
    {
        m_commandList->setViewport(m_swapchain->getSize());
        m_commandList->bindGraphicsPipelineState(*m_graphicsPipelineState);

        m_commandList->bindVertexBuffer(*m_vertexBuffer);
        m_commandList->bindIndexBuffer(*m_indexBuffer);

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

void SandboxExample::createTexture() {
    base::Image containerImage("assets/container.jpg");

    Texture2DDescriptor textureDescriptor{};
    textureDescriptor.format = ImageFormat::RGB8_UNORM;
    textureDescriptor.size = containerImage.size;
    m_texture = m_renderer->createTexture2D(textureDescriptor);

    TextureWriteDescriptor textureWriteDescriptor{};
    textureWriteDescriptor.data = containerImage.data;
    textureWriteDescriptor.dataFormat = ImageFormat::RGB8UI;
    textureWriteDescriptor.subresource.baseMipmapLevel = 0u;
    textureWriteDescriptor.size = {containerImage.size.x(), containerImage.size.y(), 0u};
    m_texture->write(textureWriteDescriptor);
}

void SandboxExample::createRenderPass() {
    RenderPassDescriptor renderPassDescriptor{};

    AttachmentDescriptor attachmentDescriptor{};
    attachmentDescriptor.format = m_swapchain->getSurfaceFormat();
    attachmentDescriptor.loadOp = AttachmentLoadOp::CLEAR;
    attachmentDescriptor.storeOp = AttachmentStoreOp::STORE;
    renderPassDescriptor.attachmentDescriptors = {
        {attachmentDescriptor},
    };

    AttachmentReference attachmentReference{};
    attachmentReference.index = 0u;

    SubpassDescriptor subpassDescriptor{};
    subpassDescriptor.colorAttachmentReferences = {
        attachmentReference,
    };
    renderPassDescriptor.subpassDescriptors = {
        subpassDescriptor,
    };

    m_renderPass = m_renderer->createRenderPass(renderPassDescriptor);
}

void SandboxExample::createFramebuffer() {
    FramebufferDescriptor framebufferDescriptor{};
    framebufferDescriptor.attachments = m_swapchain->getSwapchainTextures();
    framebufferDescriptor.renderPass = m_renderPass.get();
    framebufferDescriptor.size = m_swapchain->getSize();
    m_framebuffer = m_renderer->createFramebuffer(framebufferDescriptor);
}

void SandboxExample::createGraphicsPipelineState() {
    GraphicsPipelineStateDescriptor graphicsPipelineStateDescriptor{};
    graphicsPipelineStateDescriptor.primitiveTopology = PrimitiveTopology::TRIANGLE_LIST;

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

    DescriptorSetLayoutBinding descriptorSetLayoutBinding{};
    descriptorSetLayoutBinding.bindingIndex = 0u;
    descriptorSetLayoutBinding.descriptorType = DescriptorType::TEXTURE;
    descriptorSetLayoutBinding.descriptorCount = 1u;

    DescriptorSetLayout descriptorSetLayout{};
    descriptorSetLayout.bindings = {
        descriptorSetLayoutBinding,
    };

    PipelineLayoutDescriptor pipelineLayoutDescriptor{};
    pipelineLayoutDescriptor.uniformDescriptors = {
        {"uOffset", UniformDataFormat::FLOAT1},

    };
    pipelineLayoutDescriptor.setLayouts = {
        descriptorSetLayout,
    };
    m_pipelineLayout = m_renderer->createPipelineLayout(pipelineLayoutDescriptor);

    // create descriptor set

    graphicsPipelineStateDescriptor.renderPass = m_renderPass.get();
    graphicsPipelineStateDescriptor.pipelineLayout = m_pipelineLayout.get();
    graphicsPipelineStateDescriptor.subpassIndex = 0u;

    m_graphicsPipelineState = m_renderer->createGraphicsPipelineState(graphicsPipelineStateDescriptor);
}

} // namespace nox
