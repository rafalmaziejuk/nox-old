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

#include <nox/config.h>

namespace NOX {

RendererBackend GLRenderer::getRendererBackend() const {
    return RendererBackend::OPENGL;
}

std::shared_ptr<Surface> GLRenderer::createSurface(const SurfaceDescriptor &descriptor) {
    m_context = GLContext::create(descriptor);
    return m_context;
}

std::unique_ptr<Swapchain> GLRenderer::createSwapchain(const SwapchainDescriptor &descriptor) {
    if (m_context == nullptr) {
        NOX_ASSERT_MSG(false, "Surface needs to be created before swapchain");
        return nullptr;
    }

    auto swapchain = std::make_unique<GLSwapchain>(descriptor, m_context);
    m_state.currentRenderTarget = &swapchain->getDefaultRenderTarget();

    return swapchain;
}

std::unique_ptr<Buffer> GLRenderer::createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &vertexFormat) {
    auto &vertexArrayRegistry = m_state.vertexArrayRegistry;
    auto vertexArrayIndex = vertexArrayRegistry.registerVertexArray(vertexFormat);

    auto buffer = std::make_unique<GLVertexBuffer>(descriptor, m_state);
    buffer->setVertexArrayIndex(vertexArrayIndex);
    vertexArrayRegistry[vertexArrayIndex].setVertexBuffer(buffer->getHandle());

    return buffer;
}

std::unique_ptr<Buffer> GLRenderer::createIndexBuffer(const BufferDescriptor &descriptor, Format format) {
    auto buffer = std::make_unique<GLIndexBuffer>(descriptor, m_state);
    buffer->setIndexType(format);

    return buffer;
}

std::unique_ptr<Shader> GLRenderer::createShader(const ShaderDescriptor &descriptor, std::string_view source) {
    auto shader = std::make_unique<GLShader>(descriptor);
    if (!shader->compile(source.data())) {
        NOX_ASSERT(false);
        return nullptr;
    }

    return shader;
}

std::unique_ptr<GraphicsPipelineState> GLRenderer::createGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor) {
    auto pipeline = std::make_unique<GLGraphicsPipelineState>(descriptor, m_state);
    if (!pipeline->bindShaderStages(descriptor.shaderStages)) {
        NOX_ASSERT(false);
        return nullptr;
    }

    return pipeline;
}

std::unique_ptr<CommandList> GLRenderer::createCommandList(const CommandListDescriptor &descriptor) {
    return std::make_unique<GLCommandList>(descriptor, m_state);
}

std::unique_ptr<Texture> GLRenderer::createTexture(const TextureDescriptor &descriptor) {
    return std::make_unique<GLTexture>(descriptor);
}

std::unique_ptr<RenderTarget> GLRenderer::createRenderTarget(const RenderTargetDescriptor &descriptor) {
    return std::make_unique<GLRenderTarget>(descriptor);
}

} // namespace NOX
