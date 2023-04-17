#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_renderer.h"

namespace NOX {

GLRenderer::GLRenderer(const RendererDescriptor &descriptor) : m_descriptor{descriptor} {
    auto config = m_descriptor.config.get<OpenGLRendererConfig>();
    m_context = std::make_shared<GLContext>(config);
}

} // namespace NOX
