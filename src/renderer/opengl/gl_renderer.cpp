#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_renderer.h"
#include "renderer/opengl/gl_swap_chain.h"

namespace NOX {

GLRenderer::GLRenderer(const RendererDescriptor &descriptor) {
    auto config = descriptor.config.get<OpenGLRendererConfig>();
    m_context = std::make_shared<GLContext>(config);
}

std::unique_ptr<SwapChain> GLRenderer::createSwapChain(const SwapChainDescriptor &descriptor, const Window &window) {
    m_context->createExtendedContext(descriptor.pixelFormatDescriptor, window);
    return std::make_unique<GLSwapChain>(descriptor, m_context);
}

} // namespace NOX
