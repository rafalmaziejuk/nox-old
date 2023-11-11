#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_swap_chain.h"

namespace NOX {

GLSwapChain::GLSwapChain(const SwapChainDescriptor &descriptor, GLContext &context) : GLWithContext{context} {
    setVSync(descriptor.isVSync);
}

void GLSwapChain::swap() const {
    getContext().swapBuffers();
}

void GLSwapChain::setVSync(bool value) {
    getContext().setSwapInterval(value);
}

} // namespace NOX
