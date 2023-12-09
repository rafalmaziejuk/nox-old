#include "nox_assert.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_command_list.h"
#include "opengl/gl_context.h"
#include "opengl/gl_graphics_pipeline_state.h"
#include "opengl/gl_render_target.h"
#include "opengl/gl_renderer.h"
#include "opengl/gl_shader.h"
#include "opengl/gl_swapchain.h"
#include "opengl/gl_texture.h"
#include "opengl/gl_vertex_array.h"

namespace NOX {

RendererBackend GLRenderer::getRendererBackend() const {
    return RendererBackend::OPENGL;
}

std::shared_ptr<Surface> GLRenderer::createSurface(const SurfaceDescriptor &descriptor) {
    NOX_ASSERT(GLContext::validateInput(descriptor));

    m_context = GLContext::create(descriptor);
    return m_context;
}

std::unique_ptr<Swapchain> GLRenderer::createSwapchain(const SwapchainDescriptor &descriptor) {
    NOX_ASSERT(GLSwapchain::validateInput(descriptor));
    NOX_ENSURE_RETURN_NULLPTR_MSG(m_context != nullptr, "Surface needs to be created before swapchain");

    auto swapchain = std::make_unique<GLSwapchain>(descriptor, m_context);
    m_state.currentRenderTarget = &swapchain->getDefaultRenderTarget();

    return swapchain;
}

std::unique_ptr<Buffer> GLRenderer::createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &vertexFormat) {
    NOX_ASSERT(GLVertexBuffer::validateInput(descriptor, vertexFormat));

    auto &vertexArrayRegistry = m_state.vertexArrayRegistry;
    auto vertexArrayIndex = vertexArrayRegistry.registerVertexArray(vertexFormat);

    auto buffer = std::make_unique<GLVertexBuffer>(descriptor, m_state);
    buffer->setVertexArrayIndex(vertexArrayIndex);
    vertexArrayRegistry[vertexArrayIndex].setVertexBuffer(buffer->getHandle());

    return buffer;
}

std::unique_ptr<Buffer> GLRenderer::createIndexBuffer(const BufferDescriptor &descriptor, Format format) {
    NOX_ASSERT(GLIndexBuffer::validateInput(descriptor, format));

    auto buffer = std::make_unique<GLIndexBuffer>(descriptor, m_state);
    buffer->setIndexType(format);

    return buffer;
}

std::unique_ptr<Shader> GLRenderer::createShader(const ShaderDescriptor &descriptor, std::string_view source) {
    NOX_ASSERT(GLShader::validateInput(descriptor, source));

    auto shader = std::make_unique<GLShader>(descriptor);
    NOX_ENSURE_RETURN_NULLPTR_MSG(shader->compile(source.data()), "Couldn't compile shader");

    return shader;
}

std::unique_ptr<GraphicsPipelineState> GLRenderer::createGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor) {
    NOX_ASSERT(GLGraphicsPipelineState::validateInput(descriptor));

    auto pipeline = std::make_unique<GLGraphicsPipelineState>(descriptor, m_state);
    NOX_ENSURE_RETURN_NULLPTR_MSG(pipeline->bindShaderStages(descriptor.shaderStages), "Couldn't bind graphics pipeline shader stages");

    return pipeline;
}

std::unique_ptr<CommandList> GLRenderer::createCommandList(const CommandListDescriptor &descriptor) {
    return std::make_unique<GLCommandList>(descriptor, m_state);
}

std::unique_ptr<Texture> GLRenderer::createTexture(const TextureDescriptor &descriptor) {
    NOX_ASSERT(GLTexture::validateInput(descriptor));

    return std::make_unique<GLTexture>(descriptor);
}

std::unique_ptr<RenderTarget> GLRenderer::createRenderTarget(const RenderTargetDescriptor &descriptor) {
    return std::make_unique<GLRenderTarget>(descriptor);
}

} // namespace NOX
