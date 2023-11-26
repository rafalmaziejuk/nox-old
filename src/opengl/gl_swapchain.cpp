#include "opengl/gl_context.h"
#include "opengl/gl_render_target.h"
#include "opengl/gl_swapchain.h"

namespace NOX {

GLSwapchain::GLSwapchain(const SwapchainDescriptor &descriptor, GLContext &context) : GLWithContext{context} {
    setVSync(descriptor.isVSync);
}

void GLSwapchain::swap() const {
    getContext().swapBuffers();
}

void GLSwapchain::setVSync(bool value) {
    getContext().setSwapInterval(value);
}

} // namespace NOX
