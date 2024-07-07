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

GLRenderer::GLRenderer()
    : m_vertexArrayRegistry{GLVertexArrayRegistry::create()} {}

RendererBackend GLRenderer::getRendererBackend() const {
    return RendererBackend::OPENGL;
}

std::unique_ptr<Swapchain> GLRenderer::createSwapchain(const SwapchainDescriptor &descriptor) {
    auto context = GLContext::create(descriptor.surfaceDescriptor);
    auto swapchain = GLSwapchain::create(descriptor, std::move(context));
    return swapchain;
}

std::unique_ptr<Buffer> GLRenderer::createVertexBuffer(const VertexBufferDescriptor &descriptor) {
    auto buffer = GLVertexBuffer::create(descriptor, m_vertexArrayRegistry);
    return buffer;
}

std::unique_ptr<Buffer> GLRenderer::createIndexBuffer(const IndexBufferDescriptor &descriptor) {
    auto buffer = GLIndexBuffer::create(descriptor, m_vertexArrayRegistry);
    return buffer;
}

std::unique_ptr<Shader> GLRenderer::createShader(const ShaderDescriptor &descriptor, std::string_view source) {
    auto shader = GLShader::create(descriptor, source.data());
    return shader;
}

std::unique_ptr<GraphicsPipelineState> GLRenderer::createGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor) {
    auto pipeline = GLGraphicsPipelineState::create(descriptor);
    return pipeline;
}

std::unique_ptr<CommandList> GLRenderer::createCommandList(const CommandListDescriptor &descriptor) {
    return std::make_unique<GLCommandList>(descriptor);
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
    auto framebuffer = GLFramebuffer::create(descriptor);
    return framebuffer;
}

std::unique_ptr<PipelineLayout> GLRenderer::createPipelineLayout(const PipelineLayoutDescriptor &descriptor) {
    auto layout = GLPipelineLayout::create(descriptor);
    return layout;
}

} // namespace nox
