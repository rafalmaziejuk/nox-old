#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_command_list.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_pass.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_renderer.h"
#include "renderer/opengl/gl_shader.h"
#include "renderer/opengl/gl_swap_chain.h"
#include "renderer/opengl/gl_texture.h"
#include "renderer/opengl/gl_vertex_array.h"

#include <glad/gl.h>

namespace NOX {

namespace {

GLenum mapPrimitiveTopologyToEnum(PrimitiveTopology topology) {
    switch (topology) {
    case PrimitiveTopology::TRIANGLE_LIST: return GL_TRIANGLES;

    default: return GL_NONE;
    }
}

} // namespace

RendererBackend GLRenderer::getRendererBackend() const {
    return RendererBackend::OPENGL;
}

std::unique_ptr<SwapChain> GLRenderer::createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) {
    m_context.createExtendedContext(descriptor.pixelFormatDescriptor, window);
    return std::make_unique<GLSwapChain>(descriptor, m_context);
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

std::unique_ptr<Shader> GLRenderer::createShaderFromString(const ShaderDescriptor &descriptor, std::string_view source) {
    auto shader = std::make_unique<GLShader>(descriptor);
    shader->compileFromString(source);

    return shader;
}

std::unique_ptr<PipelineState> GLRenderer::createPipelineState(const PipelineStateDescriptor &descriptor) {
    m_state.primitiveTopology = mapPrimitiveTopologyToEnum(descriptor.primitiveTopology);
    return std::make_unique<GLPipelineState>(descriptor);
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

std::unique_ptr<RenderPass> GLRenderer::createRenderPass(const RenderPassDescriptor &descriptor) {
    return std::make_unique<GLRenderPass>(descriptor);
}

} // namespace NOX
