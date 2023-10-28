#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_swap_chain.h"

namespace NOX {

GLSwapChain::GLSwapChain(const SwapChainDescriptor &descriptor, std::shared_ptr<GLContext> context) : m_context{std::move(context)},
                                                                                                      m_renderTarget{std::make_shared<GLDefaultRenderTarget>()} {
    setVSync(descriptor.isVSync);
}

void GLSwapChain::swap() const {
    m_context->swapBuffers();
}

void GLSwapChain::setVSync(bool value) {
    m_context->setSwapInterval(value);
}

std::shared_ptr<RenderTarget> GLSwapChain::getRenderTarget() {
    return m_renderTarget;
}

} // namespace NOX
