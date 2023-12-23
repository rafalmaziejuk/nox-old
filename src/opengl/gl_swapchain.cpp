#include "nox_assert.h"
#include "opengl/gl_context.h"
#include "opengl/gl_swapchain.h"

#include <glad/gl.h>

namespace nox {

namespace {

ImageFormat queryDefaultFramebufferImageFormat() {
    constexpr GLuint defaultFramebufferHandle = 0u;
    ImageFormat result = ImageFormat::RGBA8_UNORM;

    GLint alphaBits = 0;
    glGetNamedFramebufferAttachmentParameteriv(defaultFramebufferHandle, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alphaBits);
    if (alphaBits == 0) {
        result = ImageFormat::RGB8_UNORM;
    }

    return result;
}

} // namespace

bool GLSwapchain::validateInput(const SwapchainDescriptor &descriptor) {
    bool result = true;

    result &= GLContext::validateInput(descriptor.surfaceDescriptor);
    result &= (std::get_if<OpenGLPresentMode>(&descriptor.presentMode) != nullptr);

    return result;
}

GLSwapchain::GLSwapchain(const SwapchainDescriptor &descriptor) : m_size{descriptor.size} {
    m_context = GLContext::create(descriptor.surfaceDescriptor);
    NOX_ASSERT(m_context != nullptr);

    const auto *presentMode = std::get_if<OpenGLPresentMode>(&descriptor.presentMode);
    m_context->setSwapInterval(presentMode->vSync);

    m_presentableTexture.setFormat(queryDefaultFramebufferImageFormat());
}

std::vector<const Texture *> GLSwapchain::getPresentableTextures() const {
    AttachmentsContainer result;
    result.resize(1u);
    result[0] = &m_presentableTexture;

    return result;
}

void GLSwapchain::present() const {
    m_context->swapBuffers();
}

ImageFormat GLSwapchain::getSurfaceFormat() const {
    return queryDefaultFramebufferImageFormat();
}

Vector2D<uint32_t> GLSwapchain::getSize() const {
    return m_size;
}

} // namespace nox
