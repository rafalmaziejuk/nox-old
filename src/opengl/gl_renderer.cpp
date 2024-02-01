#include "asserts.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_command_list.h"
#include "opengl/gl_context.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_graphics_pipeline_state.h"
#include "opengl/gl_pipeline_layout.h"
#include "opengl/gl_render_pass.h"
#include "opengl/gl_renderer.h"
#include "opengl/gl_shader.h"
#include "opengl/gl_swapchain.h"
#include "opengl/gl_texture.h"
#include "opengl/gl_vertex_array.h"

namespace nox {

RendererBackend GLRenderer::getRendererBackend() const {
    return RendererBackend::OPENGL;
}

std::unique_ptr<Swapchain> GLRenderer::createSwapchain(const SwapchainDescriptor &descriptor) {
    NOX_ASSERT(GLSwapchain::validateInput(descriptor));

    return std::make_unique<GLSwapchain>(descriptor);
}

std::unique_ptr<Buffer> GLRenderer::createVertexBuffer(const VertexBufferDescriptor &descriptor) {
    NOX_ASSERT(GLVertexBuffer::validateInput(descriptor));

    auto &vertexArrayRegistry = m_state.vertexArrayRegistry;
    if (!vertexArrayRegistry.contains(descriptor.vertexAttributes)) {
        vertexArrayRegistry.registerVertexArray(descriptor.vertexAttributes);
    }

    auto vertexArrayIndex = vertexArrayRegistry.find(descriptor.vertexAttributes);
    auto buffer = std::make_unique<GLVertexBuffer>(descriptor, m_state);
    buffer->setVertexArrayIndex(vertexArrayIndex);
    vertexArrayRegistry[vertexArrayIndex].setVertexBuffer(buffer->getHandle());

    return buffer;
}

std::unique_ptr<Buffer> GLRenderer::createIndexBuffer(const IndexBufferDescriptor &descriptor) {
    NOX_ASSERT(GLIndexBuffer::validateInput(descriptor));

    auto buffer = std::make_unique<GLIndexBuffer>(descriptor, m_state);
    buffer->setIndexType(descriptor.format);

    return buffer;
}

std::unique_ptr<Shader> GLRenderer::createShader(const ShaderDescriptor &descriptor, std::string_view source) {
    NOX_ASSERT(GLShader::validateInput(descriptor, source));

    auto shader = std::make_unique<GLShader>(descriptor);
    NOX_ENSURE_RETURN_NULLPTR_MSG(shader->compile(source.data()), "Couldn't compile shader");

    return shader;
}

std::unique_ptr<PipelineLayout> GLRenderer::createPipelineLayout(PipelineLayoutDescriptor &descriptor) {
    NOX_ASSERT(GLPipelineLayout::validateInput(descriptor));

    return std::make_unique<GLPipelineLayout>(descriptor);
}

std::unique_ptr<GraphicsPipelineState> GLRenderer::createGraphicsPipelineState(GraphicsPipelineStateDescriptor &descriptor) {
    NOX_ASSERT(GLGraphicsPipelineState::validateInput(descriptor));

    auto pipeline = std::make_unique<GLGraphicsPipelineState>(descriptor, m_state);
    NOX_ENSURE_RETURN_NULLPTR_MSG(pipeline->bindShaderStages(descriptor.shaderStages),
                                  "Couldn't bind graphics pipeline shader stages");

    return pipeline;
}

std::unique_ptr<CommandList> GLRenderer::createCommandList(const CommandListDescriptor &descriptor) {
    return std::make_unique<GLCommandList>(descriptor, m_state);
}

std::shared_ptr<Texture> GLRenderer::createTexture2D(const Texture2DDescriptor &descriptor) {
    NOX_ASSERT(GLTexture2D::validateInput(descriptor));

    return std::make_shared<GLTexture2D>(descriptor);
}

std::unique_ptr<RenderPass> GLRenderer::createRenderPass(const RenderPassDescriptor &descriptor) {
    NOX_ASSERT(GLRenderPass::validateInput(descriptor));

    return std::make_unique<GLRenderPass>(descriptor);
}

std::unique_ptr<Framebuffer> GLRenderer::createFramebuffer(const FramebufferDescriptor &descriptor) {
    NOX_ASSERT(GLFramebuffer::validateInput(descriptor));

    std::unique_ptr<GLFramebufferBase> framebuffer{nullptr};
    if (GLFramebufferBase::isDefaultFramebuffer(descriptor.attachments)) {
        framebuffer = std::make_unique<GLDefaultFramebuffer>();
    } else {
        framebuffer = std::make_unique<GLFramebuffer>(descriptor);
    }

    NOX_ENSURE_RETURN_NULLPTR_MSG(framebuffer->validate(), "Framebuffer isn't valid");

    return framebuffer;
}

} // namespace nox
