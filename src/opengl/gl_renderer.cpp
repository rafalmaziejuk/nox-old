#include "nox_assert.h"
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

std::unique_ptr<Swapchain> GLRenderer::createSwapchain(const SwapchainDescription &description) {
    NOX_ASSERT(GLSwapchain::validateInput(description));

    return std::make_unique<GLSwapchain>(description);
}

std::unique_ptr<Buffer> GLRenderer::createVertexBuffer(const VertexBufferDescription &description) {
    NOX_ASSERT(GLVertexBuffer::validateInput(description));

    auto &vertexArrayRegistry = m_state.vertexArrayRegistry;
    if (!vertexArrayRegistry.contains(description.vertexAttributes)) {
        vertexArrayRegistry.registerVertexArray(description.vertexAttributes);
    }

    auto vertexArrayIndex = vertexArrayRegistry.find(description.vertexAttributes);
    auto buffer = std::make_unique<GLVertexBuffer>(description, m_state);
    buffer->setVertexArrayIndex(vertexArrayIndex);
    vertexArrayRegistry[vertexArrayIndex].setVertexBuffer(buffer->getHandle());

    return buffer;
}

std::unique_ptr<Buffer> GLRenderer::createIndexBuffer(const IndexBufferDescription &description) {
    NOX_ASSERT(GLIndexBuffer::validateInput(description));

    auto buffer = std::make_unique<GLIndexBuffer>(description, m_state);
    buffer->setIndexType(description.format);

    return buffer;
}

std::unique_ptr<Shader> GLRenderer::createShader(const ShaderDescription &description, std::string_view source) {
    NOX_ASSERT(GLShader::validateInput(description, source));

    auto shader = std::make_unique<GLShader>(description);
    NOX_ENSURE_RETURN_NULLPTR_MSG(shader->compile(source.data()), "Couldn't compile shader");

    return shader;
}

std::unique_ptr<PipelineLayout> GLRenderer::createPipelineLayout(PipelineLayoutDescription &description) {
    NOX_ASSERT(GLPipelineLayout::validateInput(description));

    return std::make_unique<GLPipelineLayout>(description);
}

std::unique_ptr<GraphicsPipelineState> GLRenderer::createGraphicsPipelineState(GraphicsPipelineStateDescription &description) {
    NOX_ASSERT(GLGraphicsPipelineState::validateInput(description));

    auto pipeline = std::make_unique<GLGraphicsPipelineState>(description, m_state);
    NOX_ENSURE_RETURN_NULLPTR_MSG(pipeline->bindShaderStages(description.shaderStages),
                                  "Couldn't bind graphics pipeline shader stages");

    return pipeline;
}

std::unique_ptr<CommandList> GLRenderer::createCommandList(const CommandListDescription &description) {
    return std::make_unique<GLCommandList>(description, m_state);
}

std::shared_ptr<Texture> GLRenderer::createTexture2D(const Texture2DDescription &description) {
    NOX_ASSERT(GLTexture2D::validateInput(description));

    return std::make_shared<GLTexture2D>(description);
}

std::unique_ptr<RenderPass> GLRenderer::createRenderPass(const RenderPassDescription &description) {
    NOX_ASSERT(GLRenderPass::validateInput(description));

    return std::make_unique<GLRenderPass>(description);
}

std::unique_ptr<Framebuffer> GLRenderer::createFramebuffer(const FramebufferDescription &description) {
    NOX_ASSERT(GLFramebuffer::validateInput(description));

    std::unique_ptr<GLFramebuffer> framebuffer{nullptr};
    if (GLFramebuffer::isDefaultFramebuffer(description.attachments)) {
        framebuffer = std::make_unique<GLDefaultFramebuffer>();
    } else {
        framebuffer = std::make_unique<GLFramebufferWithAttachments>(description);
    }

    NOX_ENSURE_RETURN_NULLPTR_MSG(framebuffer->validate(), "Framebuffer isn't valid");

    return framebuffer;
}

} // namespace nox
