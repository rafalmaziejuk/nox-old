#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_swap_chain.h"

namespace NOX {

GLSwapChain::GLSwapChain(const SwapChainDescriptor &descriptor, GLContext &context) : GLWithContext{context},
                                                                                      m_renderTarget{std::make_shared<GLDefaultRenderTarget>()} {
    setVSync(descriptor.isVSync);
}

void GLSwapChain::swap() const {
    getContext().swapBuffers();
}

void GLSwapChain::setVSync(bool value) {
    getContext().setSwapInterval(value);
}

std::shared_ptr<RenderTarget> GLSwapChain::getRenderTarget() {
    return m_renderTarget;
}

} // namespace NOX
