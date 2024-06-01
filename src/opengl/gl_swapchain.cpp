#include "asserts.h"
#include "opengl/gl_context.h"
#include "opengl/gl_shader.h"
#include "opengl/gl_swapchain.h"
#include "opengl/gl_vertex_array.h"

#include <glad/gl.h>

namespace nox {

namespace {

ImageFormat queryDefaultFramebufferImageFormat() {
    constexpr GLuint defaultFramebufferHandle = 0u;

    GLint alphaBits = 0;
    glGetNamedFramebufferAttachmentParameteriv(defaultFramebufferHandle, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alphaBits);

    return ((alphaBits != 0) ? ImageFormat::RGBA8_UNORM : ImageFormat::RGB8_UNORM);
}

constexpr auto presentVertexShaderSource = R"(
            #version 460 core

            out gl_PerVertex {
	            vec4 gl_Position;
            };

		    out vec2 textureCoordinates;

		    void main() {
                const vec2 positions[4] = vec2[](
                    vec2(-1.0, -1.0),
                    vec2( 1.0, -1.0),
                    vec2(-1.0,  1.0),
                    vec2( 1.0,  1.0)
                );
                const vec2 coordinates[4] = vec2[](
                    vec2(0.0, 0.0),
                    vec2(1.0, 0.0),
                    vec2(0.0, 1.0),
                    vec2(1.0, 1.0)
                );

			    textureCoordinates = coordinates[gl_VertexID];
			    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0); 
		    }
        )";

constexpr auto presentFragmentShaderSource = R"(
            #version 460 core

            layout(binding = 0) uniform sampler2D fullscreenTexture;

            in vec2 textureCoordinates;
            out vec4 fragmentColor;

            void main() {
                fragmentColor = texture(fullscreenTexture, textureCoordinates);
            }
        )";

} // namespace

std::unique_ptr<GLSwapchain> GLSwapchain::create(const SwapchainDescriptor &descriptor, std::unique_ptr<GLContext> context) {
    const auto *presentMode = std::get_if<OpenGLPresentMode>(&descriptor.presentMode);
    NOX_ENSURE_RETURN_NULLPTR(presentMode != nullptr);
    context->setSwapInterval(presentMode->vSync);

    auto swapchain = std::make_unique<GLSwapchain>(std::move(context), descriptor.size);
    NOX_ENSURE_RETURN_NULLPTR(swapchain->initializePresentationProgram());

    return swapchain;
}

GLSwapchain::GLSwapchain(std::unique_ptr<GLContext> context, Vector2D<uint32_t> size)
    : m_context{std::move(context)},
      m_presentationTexture{{{queryDefaultFramebufferImageFormat()}, size}},
      m_size{size} {}

std::vector<const Texture *> GLSwapchain::getSwapchainTextures() const {
    return {1u, &m_presentationTexture};
}

void GLSwapchain::present() const {
    const auto &vertexArrayRegistry = GLVertexArrayRegistry::instance();
    const auto &emptyVertexArray = vertexArrayRegistry.getVertexArray(0u);

    emptyVertexArray.bind();
    m_presentationTexture.bind(0u);

    m_presentationProgram.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_presentationProgram.unbind();

    m_context->swapBuffers();
}

ImageFormat GLSwapchain::getSurfaceFormat() const {
    return m_presentationTexture.getFormat();
}

Vector2D<uint32_t> GLSwapchain::getSize() const {
    return m_size;
}

bool GLSwapchain::initializePresentationProgram() {
    const auto presentVertexShader = GLShader::create({ShaderType::VERTEX});
    NOX_ENSURE_RETURN_FALSE(presentVertexShader != nullptr);
    NOX_ENSURE_RETURN_FALSE(presentVertexShader->compile(presentVertexShaderSource));

    const auto presentFragmentShader = GLShader::create({ShaderType::FRAGMENT});
    NOX_ENSURE_RETURN_FALSE(presentFragmentShader != nullptr);
    NOX_ENSURE_RETURN_FALSE(presentFragmentShader->compile(presentFragmentShaderSource));

    m_presentationProgram.attachShader(presentVertexShader->getHandle());
    m_presentationProgram.attachShader(presentFragmentShader->getHandle());
    NOX_ENSURE_RETURN_FALSE(m_presentationProgram.link());

    return true;
}

} // namespace nox
