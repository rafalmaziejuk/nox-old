#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_swap_chain.h"

namespace NOX {

GLSwapChain::GLSwapChain(const SwapChainDescriptor &descriptor, std::shared_ptr<GLContext> &context) : m_context{context} {
    setVSync(descriptor.isVSync);
    NOX_LOG_INFO(OPENGL, "Created swap chain");
}

void GLSwapChain::swap() const {
    m_context->swapBuffers();
}

void GLSwapChain::setVSync(bool value) {
    m_context->setSwapInterval(value);
}

} // namespace NOX
