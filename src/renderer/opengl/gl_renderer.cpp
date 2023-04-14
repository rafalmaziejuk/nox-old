#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_renderer.h"
#include "renderer/opengl/gl_swap_chain.h"

namespace NOX {

GLRenderer::GLRenderer(const RendererDescriptor &descriptor) : m_descriptor{descriptor} {
    auto config = m_descriptor.config.get<OpenGLRendererConfig>();
    m_context = std::make_shared<GLContext>(config);
}

std::shared_ptr<SwapChain> GLRenderer::createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) {
    NOX_ASSERT_MSG(m_swapChain != nullptr, "Only one swap chain per renderer is currently supported");

    m_context->createExtendedContext(descriptor.pixelFormatDescriptor, window);
    m_swapChain = std::make_shared<GLSwapChain>(descriptor, m_context);

    return m_swapChain;
}

} // namespace NOX
