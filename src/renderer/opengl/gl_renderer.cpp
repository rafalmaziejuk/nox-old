#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_command_list.h"
#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_pass.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_renderer.h"
#include "renderer/opengl/gl_shader.h"
#include "renderer/opengl/gl_state.h"
#include "renderer/opengl/gl_swap_chain.h"
#include "renderer/opengl/gl_texture.h"
#include "renderer/opengl/gl_vertex_array.h"

namespace NOX {

namespace {

GLenum mapPrimitiveTopologyToEnum(PrimitiveTopology topology) {
    switch (topology) {
    case PrimitiveTopology::TRIANGLE_LIST:
        return GL_TRIANGLES;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

} // namespace

GLRenderer::GLRenderer(const RendererDescriptor &descriptor) {
    auto config = descriptor.config.get<OpenGLRendererConfig>();
    m_context = std::make_shared<GLContext>(config);
    m_state = std::make_shared<GLState>();
}

std::unique_ptr<SwapChain> GLRenderer::createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) {
    m_context->createExtendedContext(descriptor.pixelFormatDescriptor, window);
    return std::make_unique<GLSwapChain>(descriptor, m_context);
}

std::unique_ptr<Buffer> GLRenderer::createVertexBuffer(const BufferDescriptor &descriptor, const VertexFormat &format) {
    auto buffer = std::make_unique<GLVertexBuffer>(descriptor);
    uint32_t vertexArrayIndex = 0u;

    if (isVertexFormatUnique(format, vertexArrayIndex)) {
        auto vertexArray = std::make_unique<GLVertexArray>();
        vertexArray->setVertexBuffer(*buffer, format);
        m_state->vertexArrays.push_back(std::move(vertexArray));
    } else {
        m_state->vertexArrays[vertexArrayIndex]->setVertexBuffer(*buffer, format);
    }

    buffer->setVertexArrayIndex(vertexArrayIndex);

    return buffer;
}

std::unique_ptr<Buffer> GLRenderer::createIndexBuffer(const BufferDescriptor &descriptor, Format format) {
    auto buffer = std::make_unique<GLIndexBuffer>(descriptor);
    buffer->setIndexType(format);

    return buffer;
}

std::unique_ptr<Shader> GLRenderer::createShaderFromString(const ShaderDescriptor &descriptor, std::string_view source) {
    auto shader = std::make_unique<GLShader>(descriptor);
    shader->compileFromString(source);

    return shader;
}

std::unique_ptr<PipelineState> GLRenderer::createPipelineState(const PipelineStateDescriptor &descriptor) {
    m_state->primitiveTopology = mapPrimitiveTopologyToEnum(descriptor.primitiveTopology);
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
    NOX_ASSERT(descriptor.pipelineState == nullptr);
    NOX_ASSERT(downcast<GLPipelineState>(*descriptor.pipelineState)->getRenderTarget() == nullptr);
    return std::make_unique<GLRenderPass>(descriptor);
}

bool GLRenderer::isVertexFormatUnique(const VertexFormat &format, uint32_t &index) {
    auto isSameVertexFormat = [](const VertexFormat &lhs, const VertexFormat &rhs) {
        if (lhs.attributes.size() != rhs.attributes.size()) {
            return false;
        }

        for (size_t i = 0; i < lhs.attributes.size(); i++) {
            if (lhs.attributes[i].format != rhs.attributes[i].format) {
                return false;
            }
        }

        return true;
    };

    for (size_t i = 0; i < m_state->vertexFormats.size(); i++) {
        if (isSameVertexFormat(m_state->vertexFormats[i], format)) {
            index = static_cast<int32_t>(i);
            return false;
        }
    }

    index = static_cast<uint32_t>(m_state->vertexArrays.size());
    m_state->vertexFormats.push_back(format);

    return true;
}

} // namespace NOX
